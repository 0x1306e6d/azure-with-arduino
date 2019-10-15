#r "Microsoft.Azure.EventHubs“

using System;
using System.Text;
using Microsoft.Azure.EventHubs;

public static void Run(EventData myIoTHubMessage, ILogger log)
{
    log.LogInformation($"Event: {Encoding.UTF8.GetString(myIoTHubMessage.Body)}");
    log.LogInformation($"EnqueuedTimeUtc: {myIoTHubMessage.SystemProperties.EnqueuedTimeUtc}");
    log.LogInformation($"SequenceNumber: {myIoTHubMessage.SystemProperties.SequenceNumber}");
    log.LogInformation($"Offset: {myIoTHubMessage.SystemProperties.Offset}");
}