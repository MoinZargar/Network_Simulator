#include<iostream>
#include "layers.h"
#include<time.h>
#include<map>
#include<vector>
using namespace std;

class physical_prompt{
  public:
    void run(){
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
      devices.push_back(EndDevices(i+1,""));
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
    cin.ignore();
    getline(cin,data);
  
   
    //pass data to sender
    devices[sender-1].getData(data);
    //broadcasting data
    h.broadcast(devices,sender);
    //transmission status
    cout<<endl;
    h.transmission(sender,reciever);
     //reciever sends back ack to hub
     cout<<endl;
    devices[sender-1].sendAck(reciever);
    //hub broadcast Ack
    h.BroadcastAck(sender,reciever);
    
    break;
  }
 }
};

class data_prompt{
  public:
  
  string generateMacAddress() {
    
    char mac[18];
    sprintf(mac, "%02X:%02X:%02X:%02X:%02X:%02X", rand() % 256, rand() % 256, rand() % 256, rand() % 256, rand() % 256, rand() % 256);
    return mac;
  }
  void run(){
    while(true){
    int size,sender,reciever;
    string data;
    vector<EndDevices> devices; 
    map<int,bool> mp;
    hub h;
    EndDevices end;
    int choice;
    Switch s;
    cout<<endl;
    cout<<"Choose Test case: "<<endl;
    cout<<endl;
    cout << "1 :" <<"Create a switch with n end devices" << endl;
    cout << "2 :"<<"Create n star topologies with m end devices each" << endl;
    cin >> choice; 
    if(choice==1){
    
     cout<<endl;
    //vector of objects of End devices
  
    cout<<"Enter  the number of end devices"<<endl;
    cin>>size;
    if(size<2){
        cout<<"There should be atleast two devices. Enter valid number"<<endl;
        continue;
    }
    int select;
    map<int,string> flow;
    flow={
      {1,"Stop and Wait ARQ"},
      {2,"Selective Repeat"}
    };
    cout<<endl;
    cout<<"Choose a Flow Control Protocol :"<<endl;
    cout<<endl;
    for(auto it:flow){
      cout<<it.first<<" : "<<it.second<<endl; 
    }
    cin>>select;
    if(select!=1 && select!=2){
      cout<<"Invalid Entry"<<endl;
      continue;
    }
    srand(time(0));
    for(int i=0;i<size;i++){
      //creating end devices
      string mac=generateMacAddress();
      devices.push_back(EndDevices(i+1,mac));
      //connexting devices with switch
      s.topology(devices[i]);
      if(i==0){
        cout<<"Connection status :"<<endl;
        cout<<endl;
      }
      s.print_connection(i);
    }
       
    end.prompt("sender",size,mp);
    cin>>sender;
    if(!mp[sender]){
      cout<<"Invalid Entry"<<endl;
      continue;
    }
    end.prompt("reciever",size,mp);
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
    cin.ignore();
    getline(cin,data);
    //pass data to sender
    devices[sender-1].getData(data);
   
    //token passing
    
    devices[sender-1].tokenCheck(devices,sender,size);
    //switch stores mapping of device id and MAC address
    s.MAC_table();
    //for Stop and wait ARQ
    if(select==1){
       devices[sender-1].StopAndWait();
       s.transmission(devices,sender,reciever);

    }
    //for Selective repeat
    else if(select==2){
      devices[sender-1].Selective_Repeat();
      s.transmission(devices,sender,reciever);
    }
    
   }
    else if(choice==2){
      vector<EndDevices> devices2;
      Switch s2;
      hub h2;
      int hubSize;
      int deviceNum;
      string data2;
      vector<hub> hub_vec;
      map<int,bool> mp2;
      cout<<endl;
      cout<<"Enter the number of hubs required"<<endl;
      cin>>hubSize;
      
     
       for(int i=0;i<hubSize;i++){
        //creating objects of hub
         hub_vec.push_back(hub(i+1));
         //connecting hub with switch
       
        s2.topology(hub_vec[i]);
        if(i==0){
          cout<<"Connection status :"<<endl;
          cout<<endl;
        }
        s2.hub_print_connection(i);
       }
       cout<<endl;
       cout<<"Enter the number of end devices to be connected to each hub"<<endl;
       cin>>deviceNum;
      if(deviceNum<2){
        cout<<"There should be atleast two devices. Enter valid number"<<endl;
        continue;
      }
      int id=1,k=0;
      for(int i=0;i<hub_vec.size();i++){
        for(int j=0;j<deviceNum;j++){
          devices2.push_back(EndDevices(id,""));
          //connecting end devices with hub
          hub_vec[i].topology(devices2[k++]);
          id++;
      }
      }
      for(int i=0;i<hub_vec.size();i++){
        cout<<endl;
        hub_vec[i].connection(i+1);

      }
      cout<<endl;
      for(int i=0;i<hub_vec.size();i++){
        //getting array of connected end devices to hub[i]
        vector<EndDevices> connected_devices=hub_vec[i].getDevices();
        //mapping of hub and devices connected to it
        s2.HubToDeviceMap(i,connected_devices);
      }
      s2.print_HubToDeviceMap();
      int total_devices=deviceNum*hubSize;
      end.prompt("sender",total_devices,mp2);
      cin>>sender;

      if(!mp2[sender]){
        cout<<"Invalid Entry"<<endl;
        continue;
      }
      end.prompt("reciever",total_devices,mp2);
      cin>>reciever;
      if(!mp2[reciever]){
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
      cin.ignore();
      getline(cin, data2);
      int source_hub=s2.findHubForDevice(sender);
      //pass data to sender
      devices2[sender-1].getData(data2);
      //hub recieves data
      hub_vec[source_hub].data=data2;
      //source hub broadcasts data
      hub_vec[source_hub].broadcast(devices2,sender);
      // //transmission status
      cout<<endl;
      hub_vec[source_hub].transmission(sender,reciever);
      //source hub sends data to switch
      int destination_hub=s2.recieveData(sender,reciever,data2);
      //destination hub broadcasts data
      hub_vec[destination_hub].broadcast(devices2,sender);
      // //transmission status
      cout<<endl;
      hub_vec[destination_hub].transmission(sender,reciever);
      //reciever sends back ack to hub
      cout<<endl;
      devices2[sender-1].sendAck(reciever);
      //destination hub broadcast Ack
      hub_vec[destination_hub].BroadcastAck(sender,reciever);
      //destination sends ack to switch
      hub_vec[destination_hub].ack=true;
      s2.recieveAck(destination_hub);
      //switch sends ack to source hub
      s2.send_Ack(source_hub);
      //source hub broadcast ACK
      hub_vec[source_hub].BroadcastAck(sender,reciever);
      break;
   }
   }
  }
};
class prompt{
   public:
   void run(){
    while(true){
    cout<<endl;
    int choice;
    map<int,string> mp;
    mp={
      {1,"Physical layer"},
      {2,"Data Link Layer"}
    };
    cout<<"Choose a layer :"<<endl;
    for(auto it:mp){
      cout<<it.first<<" : "<<it.second<<endl; 
    }
    cin>>choice;
    
    switch (choice)
    {
    case 1:
    {
      physical_prompt p;
      p.run();
      break;
    }
    case 2:
    {
      data_prompt d;
      d.run();
      break;
    }
    default:
    cout<<"Invalid Entry"<<endl;
      break;
    }
    
    }
   }
};
  