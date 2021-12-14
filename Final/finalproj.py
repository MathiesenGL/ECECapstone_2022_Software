import requests
from awscrt import io, mqtt
from awsiot import mqtt_connection_builder
import sys
import datetime
import time
import json
import array
import nfci2c

import config

def onConnectionInterrupted(connection, error, **kwargs):
    """Callback when connection is accidentally lost
    """
    print("AWS Connection interrupted. error: {}".format(error))
    
def onConnectionResumed(connection, returnCode, sessionPresent, **kwargs):
    """Callback when an interrupted connection is re-established
    """
    
    print("Connection resumed. returnCode: {} sessionPresent: {}".format(returnCode, sessionPresent))
    
    if returnCode == mqtt.ConnectReturnCode.ACCEPTED and not sessionPresent:
        print("Session did not persist. Resubscribing to existing topics...")
        resubscribeFuture, _ = connection.resubscribe_existing_topics()
        
        # cannot synchronously wait for resubscribe result because we're on
        # the connection's event-loop thread. Have to evalue results with
        # a callback function instead
        resubscribeFuture.add_done_callback(onResubscribeComplete)
        
def onResubscribeComplete(resubscribeFuture):
    """ Callback when re-subscribe has completed
    """
    
    results = resubscribeFuture.result()
    print("Resubscribe results: {}".format(results))
    
    for topic, qos in results['topics']:
        if qos is None:
            sys.exit("AWS server rejected resubscribe to topic: {}".format(topic))
            
def onMessageReceived(topic, payload, dup, qos, retain, **kwargs):
    """Callback when subscribed topic received a message
    """
    
    print("Received message from topic '{}': {}".format(topic, payload))
    


def main():
     # this is needed for background thread to process AWS comms
    eventLoopGroup = io.EventLoopGroup(1)    # use one thread
    hostResolver = io.DefaultHostResolver(eventLoopGroup)
    clientBootstrap = io.ClientBootstrap(eventLoopGroup, hostResolver)
    
    # set up connection to AWS
    mqttConnection = mqtt_connection_builder.mtls_from_path(
        endpoint=config.AWSIoTEndpoint,
        cert_filepath=config.AWSCertCrt,
        pri_key_filepath=config.AWSCertPrivate,
        client_bootstrap=clientBootstrap,
        ca_filepath=config.AWSCertCa,
        on_connection_interrupted=onConnectionInterrupted,
        on_connection_resumed=onConnectionResumed,
        client_id=config.AWSThingName,
        clean_session=False,
        keep_alive_secs=6)
    
    # now try to connect    
    print("Connecting to {} with client ID '{}'...".format(
        config.AWSIoTEndpoint, config.AWSThingName))
    
    connectFuture = mqttConnection.connect()
    
    # wait until a result is available
    connectFuture.result()
    print("Connected!")
    
    # Send the link to the hex file
    # get current time (UTC)
    timestamp = datetime.datetime.now(datetime.timezone.utc).ctime()
    payload = json.dumps({"timestamp": timestamp, "code": 'https://raw.githubusercontent.com/SlappyMrFlappy/SlappyMrFlappy/main/IOT_Test/IOT_Test.ino.with_bootloader.mega.hex'})
    #payload = json.dumps({"timestamp": timestamp, "code": tagID})
                
    print("Publishing message to topic '{}': {}".format(
        config.OTATopic, payload))
                    
    # publish the tag ID
    mqttConnection.publish(
        topic=config.OTATopic,
        payload=payload,
        qos=mqtt.QoS.AT_LEAST_ONCE)
    


if __name__ == '__main__':
    main()
