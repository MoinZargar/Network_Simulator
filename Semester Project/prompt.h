#include<iostream>
#include "layers.h"
#include<map>
#include<vector>
using namespace std;
class prompt{
    public:
    void display(){
    while(true){
    int d,sender,reciever;
    string data;
    map<int,bool> mp;
    hub h;
    EndDevices end;
    //vector of objects of End devices
    vector<EndDevices> devices; 
    cout<<"Physical layer implementation"<<endl;
    cout<<endl;
    cout<<"Enter  the number of end devices"<<endl;
    cin>>d;
    if(d<2){
        cout<<"There should be atleast two devices. Enter valid number"<<endl;
        continue;
    }
    
    for(int i=0;i<d;i++){
      //creating end devices
      devices.push_back(EndDevices(i+1));
      //connecting end devices with hub
      h.topology(devices[i]);
      if(i==0){
        cout<<"Connection status :"<<endl;
        cout<<endl;
      }
      h.print_connection(i);
    }
    
   
    end.prompt("sender",d,mp);
    cin>>sender;
    if(!mp[sender]){
      cout<<"Invalid Entry"<<endl;
      continue;
    }
    end.prompt("reciever",d,mp);
    cin>>reciever;
    if(!mp[reciever]){
      cout<<"Invalid Entry"<<endl;
      continue;
    }
    //if sender and reciever are same
    if(sender==reciever){
      cout<<"Sender and reciever can't be same "<<endl;
      continue;
    }
    cout<<endl;
    cout<<"Input the message that you would like to send "<<endl;
    cin>>data;
    //pass data to sender
    devices[sender-1].sendData(data);
    //broadcasting data
    h.broadcast(devices,sender);
    //transmission status
    cout<<endl;
    cout<<"Transmission Status: "<<endl;
    cout<<endl;
    h.transmission(sender,reciever);
     //sender sends ack to hub
     cout<<endl;
    devices[sender-1].sendAck(sender);
    //hub broadcast Ack
    h.BroadcastAck(sender,reciever);
    break;
  }
 }
};
