////////////////////////////////////////////////////////////////////
//
// Copyright 2019 Samsung Electronics All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
// either express or implied. See the License for the specific
// language governing permissions and limitations under the License.
//
////////////////////////////////////////////////////////////////////

package smartfs_dump_parser.parts;

import javax.annotation.PostConstruct;

import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Text;

public class GeometryViewer {

	private Label sectorSizeLabel;
	private Label sectorNumLabel;
	private Text sectorSizeText;
	private Text sectorNumText;

	public void updateGeometry(int sectorSize, int numberOfSectors) {
		sectorSizeText.setText(sectorSize + "");
		sectorSizeText.redraw();
		sectorSizeText.setEditable(false);
		
		sectorNumText.setText(numberOfSectors + "");
		sectorNumText.redraw();
		sectorNumText.setEditable(false);
	}
	
	@PostConstruct
	public void createComposite(Composite parent) {
		parent.setLayout(new GridLayout(2, true));

		sectorSizeLabel = new Label(parent, SWT.BORDER);
		sectorSizeLabel.setText("Sector Size: ");
		sectorSizeLabel.setSize(50, 20);
		sectorSizeText = new Text(parent, 0);
		sectorSizeText.setSize(50, 20);
		
		sectorNumLabel = new Label(parent, SWT.BORDER);
		sectorNumLabel.setText("Number of Sectors: ");
		sectorNumText = new Text(parent, 0);
		sectorNumText.setSize(80, 40);
	}
}
