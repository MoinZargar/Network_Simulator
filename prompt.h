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
    cout<<"Transmission Status: "<<endl;
    cout<<endl;
    h.transmission(sender,reciever);
     //sender sends ack to hub
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
    cout << "2 :"<<"Create two star topologies with n end devices each" << endl;
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
    
    }
    else{
      continue;
    }
   }
    break;
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
  