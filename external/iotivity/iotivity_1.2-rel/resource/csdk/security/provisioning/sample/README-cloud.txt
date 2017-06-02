# Cloud Connection

# Start cloud application
# Run ./cloudClient command in console. By default if uses cloud.dat database
# if you want to use custom database, run ./cloudClient <filename>

# After cloudClient start, you will see the big Menu
# First of all, you need to fill proper connection settings
# Enter number 4 (Change default host) - and enter your ip address
# By default, cloudClient using non-secure connection. If you want to use secure connection
# Enter number 5 (Change default port) - and enter 5684 (default CoAPs port)
# Enter number 10 (Change Protocol type (CoAP/CoAPs)) - and enter 1 - secure connection
# You can check all settings, they are printed in title of the Menu

# After basic setting were done, it is required to execute Sign Up request
# Enter number 1 (Sign Up request) and follow the comments (go to link, get authentication token and paste it to console)
# Then, before each user request, it is required to execute Sign In request
# Enter number 2 (Sign In request) and check the result.
# If it says "UUID is missing, execute Sign Up first", then previous Sign Up failed.
# Otherwice, it is OK and you can proceed to user requests

# After successful Sign Up and Sign In, you can execute user Requests
# There are 3 main group of requests - Certificate Request, CRL and ACL requests
# After Certificate Request, CRL Get and ACL Get Individual Info SVR database will be updated by received data from cloud
# Details about all requests you can find in latest Convergence Spec (what will be sent to and received from Cloud)

# Device-to-Device connection

# Start both ./cloudClient <client database> and ./cloudServer <server database> in two terminals
# In cloudClient Menu, Enter number 13 (Start Discovery). It will discover all resources in network.
# After discover, you can execute (14/15/16) GET/PUT/POST requests.
# Data for those requests will be automatically get from discovery (host, port and uri of first found secure resource)
