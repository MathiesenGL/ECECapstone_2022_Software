# Configuration values

# Use your own custom AWS IoT endpoint here
AWSIoTEndpoint = "a2ld6z17v34jrq-ats.iot.us-east-1.amazonaws.com"

# use the filenames for your own cert's
AWSCertCa = "/home/pi/certs/AmazonRootCA1.pem"
AWSCertCrt = "/home/pi/certs/368d80859a6b8921c7f154cc2838ae45e3b23ced6e24e24a2fa72aaea61f72a9-certificate.pem.crt"
AWSCertPrivate = "/home/pi/certs/368d80859a6b8921c7f154cc2838ae45e3b23ced6e24e24a2fa72aaea61f72a9-private.pem.key"

# MQTT topic for publishing a tag
TagTopic = "OTA/file"

# MQTT topic for receiving status updates
StatusTopic = "OTA/response"

# AWS Thing name
AWSThingName = "EE419_Pi"
