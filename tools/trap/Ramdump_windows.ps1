#*******************************************************************
#
# Copyright 2019 Samsung Electronics All Rights Reserved.
#
# File Name : Ramdump_windows.ps1
# Description: Receive Ramdump using UART
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
# either express or implied. See the License for the specific
# language governing permissions and limitations under the License.
#
#*****************************************************************/

# ------------------------------  PARAMETERS ---------------------------------
$BAUDRATE = 115200
$Parity = [System.IO.Ports.Parity]::None # System.IO.Ports.Parity
$dataBits = 8
$StopBits = [System.IO.Ports.StopBits]::one # System.IO.Ports.StopBits
$Handshake = [System.IO.Ports.Handshake]::None
# ------------------------ END COM PORT PARAMETERS ----------------------------
$getTimestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss.fff"
$index
$regions_to_dump
$KB_CHECK_COUNT = (16 * 1024)
# ------------------------------ END PARAMETERS ---------------------------------

function scan_input {
	# Check if user's input is valid (EOF pending)
	if($index -le 0 -Or ($number_of_regions -eq 1 -And $index -gt 1) -Or $index -gt ($number_of_regions + 1)) {
		$index = Read-Host -Prompt 'Please enter correct input '
	}

	$check_idx = $index
	while($check_idx -gt 0) {
		if (($check_idx % 10) -gt ($number_of_regions + 1)) {
			scan_input
		}
		$check_idx = $check_idx / 10
		$check_idx = [math]::Round($check_idx)
	}
}

function ramdump_init_recv {

	# Receive number of memory regions from TARGET
	$c = New-Object byte[] 4
	$bytesRead=$port.Read($c, 0, 1)

	$number_of_regions = [System.BitConverter]::ToUInt32($c, 0)
	#Write-Output ("[" + $getTimestamp + "] " + "No of regions = " + $number_of_regions)

	# Receive memory address, size & heap index for memory regions from TARGET to mem_info array
	# $mem_info = @($regionx_idx, $regionx_start, $regionx_size, $regionx_mark)
	$mem_info = @()

	For ($i = 2; $i -le $number_of_regions + 1; $i++) {

		$mem_address = New-Object byte[] 4
		$bytesRead = $port.Read($mem_address,0,$mem_address.Length)

		[System.Array]::Reverse($mem_address)
		$regionx_start = ($mem_address|ForEach-Object ToString X2) -join ''
		#Write-Output ("[" + $getTimestamp + "] " + "Address = 0x" + $regionx_start)

		$mem_size = New-Object byte[] 4
		$bytesRead = $port.Read($mem_size,0,$mem_size.Length)

		$regionx_size = [System.BitConverter]::ToUInt32($mem_size, 0)
		#Write-Output ("[" + $getTimestamp + "] " + "Size = "+ $regionx_size)

		$c = New-Object byte[] 4
		$bytesRead=$port.Read($c, 0, 1)
		$regionx_idx = [System.BitConverter]::ToUInt32($c, 0)
		#Write-Output ("[" + $getTimestamp + "] " + "Heap Index = "+ $regionx_idx)

		# Add read data to mem_info array
		$mem_info += ,@([pscustomobject]@{regionx_idx = $regionx_idx; regionx_start = $regionx_start; regionx_size =  $regionx_size; regionx_mark = 0})
	}

	# Display memory region options for user to dump
	Write-Output ("=========================================================================")
	Write-Output ("Ramdump Region Options:")
	Write-Output ("1. ALL")
	Write-Host -nonewline

	if($number_of_regions -eq 1){
		Write-Output ("( Address: 0x" + $mem_info[0].regionx_start + ", Size: " + $mem_info[0].regionx_size + ")" + 
									"	[Heap index = " + $mem_info[0].regionx_idx + "]")
	} else {
		$i = 0
		foreach($j in $mem_info){
			Write-Host ($i + 2) -nonewline
			Write-Output (". Region : " + $i + " ( Address: 0x" + $j.regionx_start + ", Size: " + $j.regionx_size + ")" + 
											"	[Heap index = " + $j.regionx_idx + "]")
			$i++
		}
	}
	Write-Output ("=========================================================================")
	Write-Output ("Please enter desired ramdump option as below:")
	Write-Output ("1 for ALL")

	if($number_of_regions -gt 1){
		Write-Output ("2 for Region 0")
		Write-Output ("25 for Region 0 & 3 ...")
	}
	Write-Output ("")

	$global:regions_to_dump = 0

	# Take user's input for desired ramdump region
	$index = Read-Host -Prompt 'Please enter your input '

	# Scan input from user
	scan_input

	# Mark regions to be dumped
	$mark_indexes = @()
	if($index -eq 1){
		For ($i = 0; $i -lt $number_of_regions; $i++) {
			$mark_indexes += $i
			$global:regions_to_dump++
		}
	} else {
		while($index -gt 0) {
			$mark_indexes += ,@($index % 10)
			$index = $index / 10
			$index = [math]::Round($index)
		}
		foreach($idx in $mark_indexes) {
			$global:regions_to_dump++
			$idx[0] = $idx[0] - 2
		}
	}

	# Send number of regions to dump to TARGET
	$port.Write($global:regions_to_dump)
	Write-Output ("`nTarget No. of Regions to be dumped received!`n")
	Write-Output ("Receiving ramdump......")

	# Remove old ramdump files if any
	Get-ChildItem $MyInvocation.MyCommand.Path | Where{$_.Name -Match "ramdump_0x"} | Remove-Item

	# Dump data region wise
	For ($i = 0; $i -lt $global:regions_to_dump; $i++) {
		Write-Output ("`nTarget Region info received!")

		#Send region index to TARGET
		$port.Write($mark_indexes[$i])

		Write-Output ("=========================================================================")
		if(($mark_indexes[$i] -eq 0) -And ($global:regions_to_dump -eq 1)) {
			Write-Output ("Dumping data: Address: 0x"+ $mem_info[0].regionx_start + ", Size:" + $mem_info[0].regionx_size +"bytes")
		} else {
			Write-Output ("Dumping Region: " + $mark_indexes[$i] + ", Address: 0x"+ $mem_info[$mark_indexes[$i]].regionx_start + 
										", Size:" + $mem_info[$mark_indexes[$i]].regionx_size +"bytes")
		}
		Write-Output ("=========================================================================")
		Write-Host "[>" -nonewline

		# Dump data of Memory REGIONx
		$count = 0
		$dump_size = [long]$mem_info[$mark_indexes[$i]].regionx_size

		$buffer = New-Object byte[] $dump_size
		$offset = 0

		try {
			while ($dump_size -gt 0l) {
				$bytesRead=$port.Read($buffer,$offset,1)

				$dump_size--
				$count++
				$offset++

				if($count % $KB_CHECK_COUNT -eq 0) {
					$count = 0
					Write-Host "`b=>" -nonewline
				}
			}	# until dump_size bytes are received
			Write-Output "]"
		}

		catch {
			Write-Error ("Receiving ramdump " + $offset + "TH byte failed ") -ErrorAction Stop
		}

		# Copy data to current directory
		Write-Output "Copying..."
		[io.file]::WriteAllBytes($MyInvocation.MyCommand.Path + 'ramdump_0x' + $mem_info[$mark_indexes[$i]].regionx_start + 
								'_0x' + $mem_info[$mark_indexes[$i]].regionx_size + '.bin',$buffer)
		Write-Output ("to " + ((Get-Item -Path ".\").FullName + '\ramdump_0x' + $mem_info[$mark_indexes[$i]].regionx_start + 
									'_0x' + $mem_info[$mark_indexes[$i]].regionx_size + '.bin'))
	}
}

function ramdump_main {

	# Take user's input for desired ramdump region
	$COM = Read-Host -Prompt 'Please enter COM port number & press enter '

	# Configure COM Port
	$port = new-Object System.IO.Ports.SerialPort COM$COM,$BAUDRATE,$Parity,$dataBits,$StopBits
	$port.Handshake = [System.IO.Ports.Handshake]::None

	# Open COM Port
	try {
		$currentDate = Get-Date -Format yyyyMMdd
		$fileExt = ".log"
		if ($OutputFile -eq "notempty") {
		    $filename = (Get-Item -Path ".\" -Verbose).FullName + "\SerialPortReader_" + $currentDate + $fileExt
		} else {
		    $filename = $OutputFile
		}

		Write-Output ("Establishing connection to the port...")
		Start-Sleep -Milliseconds 1000
		$port.Open()
		Write-Output $port
		Write-Output ("--> Connection established.`n")
	}

	catch [System.Exception] {
		Write-Error ("Failed to connect : " + $_)
		$error[0] | Format-List -Force
		if ($null -ne $port) {
			$port.Close()
		}
		exit 1
	}

	# Send handshake command ("RAMDUMP") to TARGET
	$data = "RAMDUMP"
	$port.Write([char[]]$data, 0, 7)

	# Read ACK from TARGET
	$buffer = New-Object char[] 1
	$bytesRead = $port.Read($buffer,0,$buffer.Length)

	# Check ACK
	if($buffer -eq 'A') {
		Write-Host Target Handshake SUCCESSFUL !!!
	}elseif ($buffer -eq 'N') {
		Write-Host Target handshake failed
	}

	Write-Host ("Target entered to ramdump mode`n")

	# Ramdump data initialization
	ramdump_init_recv

	Write-Output "`nRamdump received successfully..!"

	if ($null -ne $port) {
		$port.Close()
	}
}

ramdump_main
