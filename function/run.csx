#r "Microsoft.Azure.EventHubs“
#r "Newtonsoft.Json“
#r "SendGrid“

using System;
using System.Text;
using Microsoft.Azure.EventHubs;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using SendGrid.Helpers.Mail;

public static SendGridMessage Run(EventData myIoTHubMessage, out object outputDocument, ILogger log)
{
    log.LogInformation($"Event: {Encoding.UTF8.GetString(myIoTHubMessage.Body)}");
    log.LogInformation($"EnqueuedTimeUtc: {myIoTHubMessage.SystemProperties.EnqueuedTimeUtc}");
    log.LogInformation($"SequenceNumber: {myIoTHubMessage.SystemProperties.SequenceNumber}");
    log.LogInformation($"Offset: {myIoTHubMessage.SystemProperties.Offset}");
    
    JObject body = JObject.Parse(Encoding.UTF8.GetString(myIoTHubMessage.Body));
    var temperature = body["temperature"];
    var humidity = body["humidity"];
    log.LogInformation($"Temperature: {temperature}, Humidity: {humidity}");

    outputDocument = new {
        temperature = temperature,
        humidity = humidity,
    };

    return null;
}