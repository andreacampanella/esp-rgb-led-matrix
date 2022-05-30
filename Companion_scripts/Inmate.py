import urllib.request, json 
with urllib.request.urlopen("https://www.emfcamp.org/schedule/2022.json") as url:
    data = json.loads(url.read().decode())
from datetime import datetime
import requests

def send_to_InvPan(message,alias):
    api_url = "http://192.168.1.91/rest/api/v1/display/alias/"+alias+"/text?show="+message
    response =  requests.post(api_url)

def get_from_InvPan(alias):
    api_url = "http://192.168.1.91/rest/api/v1/display/alias/"+alias+"/text"
    response =  requests.get(api_url)
    print(response)

def on_now(date,time):
    event_start = 0
    event_end = 0 
    event_title = 0 
    event_date = 0
    for elements in data:
        event_date = elements["start_date"]
        event_start = elements["start_time"]
        event_end = elements["end_time"]
        event_title = elements["title"]
        event_location = elements["venue"]
        if event_date == date:
            if event_start <= time <= event_end:
                send_to_InvPan(event_location+", "+event_start+": "+event_title,event_location.replace(" ","_",1))
                print(event_location+", "+event_start+": "+event_title)
                #get_from_InvPan(event_location.replace(" ","_",1))

#main 
print("Inverse Panopticon Midway Bridge")

now = datetime.now()

#current_time = now.strftime("%H:%M")
#current_date = now.strftime("%Y-%m-%d %H:%M:%S")
current_time = "10:40"
current_date = "2022-06-04 10:40:00"
on_now(current_date,current_time)