#include<iostream>
#include <unistd.h>
#include<chrono>
#include<map>
#include<vector>
using namespace std;
 
class EndDevices{
   private:
   int deviceId;
   string MAC_Address;
   string message;
   public:
    int sender_buffer;
    int reciever_buffer;
    bool ack;
    bool token;
    EndDevices() {
        deviceId = 0;
        MAC_Address="";
        ack=false;
        token=false;
    }
    EndDevices(int Id,string mac){
        deviceId=Id;
        MAC_Address=mac;
    }
    int getId(){
        return deviceId;
    }
    string getMAC(){
        return MAC_Address;
    }
    void getData(string data){
        message=data;
       
       
    }
   string SendData(){
    return message;
   }

   void sendAck(int reciever){
     ack=true;
     cout<<"Device "<<reciever<<" sends back ACK "<<endl;
     
   }
    void tokenCheck(vector<EndDevices> devices,int sender,int size){
      
      cout<<endl;
      cout<<"Token status :"<<endl;
      int i=rand()%size;
      //until sender doesn't have access to token
      while(devices[sender-1].token!=true){
        //logical ring
         int Current_device=(i%size);
         devices[Current_device].token=true;
         if(Current_device!=sender-1){
         cout<<"Currently sender doesn't have access to channel. Token is at device "<<Current_device+1<<" .Waiting to get access "<<endl;
         }
         i++;
         sleep(3);
      }
      cout<<"Sender has access to channel now"<<endl;
            
    }
    void sender(vector<int> frame){
       //window sliding 
       int i=0;
       while(i<frame.size()){
          ack=false;
           auto start_time = std::chrono::system_clock::now();
          chrono::seconds timeout_duration(4);
          auto timeout_time = start_time + timeout_duration;
           sender_buffer=frame[i];
           //sending packet to reciever
           int AckNo;
           sleep(3);
           AckNo=reciever(frame,i);
           if(chrono::system_clock::now() < timeout_time){
             if(ack==true){
              cout<<"ACK "<<AckNo<<endl;
             }
             else{
                cout<<"ACK lost"<<endl;
              }
           }
           
           i++;
       }
       
    }
    int reciever(vector<int> frame,int i){
       
        cout<<"Sender sends packet with sequence number "<<sender_buffer<<endl;
        if(sender_buffer==frame[i]){
        reciever_buffer=sender_buffer;
         ack=true;
        return frame[i+1];
        }

    }
    //Flow control Protocol
      void StopAndWait(){
        int frameSize=5;
        vector<int> frame;
        for(int i=0;i<frameSize;i++){
          int random=rand()%2;
          frame.push_back(random);
        }
        sender(frame);
        
      }

    void prompt(string DeviceType,int d,map<int,bool> &mp){
     
     for(int i=1;i<=d;i++){
      mp[i]=true;
     }
     cout<<endl;
     cout<<"Choose the "<<DeviceType<< " device"<<endl;
     for(int i=0;i<mp.size();i++){
      cout<<i+1<<" : "<<"device "+to_string(i+1)<<endl;
     }
    }
   
};

class hub{
  private:
  vector<EndDevices> connected_devices;        //vector for storing endDevice objects in hub
  public:
  void topology(EndDevices devices){
    //connecting end devices to hub
    connected_devices.push_back(devices);
  } 
  void print_connection(int i){
    cout<<"Connection successfully created between hub and device "<<connected_devices[i].getId()<<endl;
  }
  void broadcast(vector<EndDevices> devices, int sender){
    cout<<endl;
    cout<<"A message is being broadcasted from the Hub"<<endl;
    cout<<endl;
    //hub is getting data from sender
    string data=devices[sender-1].SendData();
   
    //hub broadcasts data 
    for(int i=0;i<connected_devices.size();i++){
      if(i!=sender-1){
      connected_devices[i].getData(data);
      }
    }
    
  }
  //status of transmission
  void transmission(int sender,int reciever){
    cout<<endl;
    cout<<"Transmission status "<<endl;
    for(int i=0;i<connected_devices.size();i++){
       string message=connected_devices[i].SendData();
       int Current_device=i+1;
       if(Current_device!=sender){
        if(Current_device!=reciever){
          cout<<message<<" was recieved by device "<<Current_device<<" but it was discarded"<<endl;
        }
        else{
          cout<<"Device "<<Current_device<<"' recieved message '"<<message<<"' successfully"<<endl;
        }
       }
    }
  }
  //broadcast Ack
  void BroadcastAck(int sender,int reciever){
     if(connected_devices[reciever-1].ack==true){
      for(int i=0;i<connected_devices.size();i++){
       int Current_device=i+1;
       if(Current_device!=sender){
         cout<<"ACK was recieved by device "<<i+1<<" but it was discarded"<<endl;
       }
       else{
        cout<<"ACK was recieved by device "<<i+1<<" and it was accepted"<<endl;
       }
      }
     }
  }
};

class Switch{
  private:
  int switchId;
  map<int,string> mac_table;
  vector<EndDevices> connected_devices;
  public:
   void topology(EndDevices devices){
    //connecting end devices to switch
    connected_devices.push_back(devices);
   } 
  void print_connection(int i){
    cout<<"Connection successfully established between switch and device with MAC_Address: "<<connected_devices[i].getMAC()<<endl;
  }
  void MAC_table(){
     
     //switch storing mapping of device id and MAC address
     for(int i=0;i<connected_devices.size();i++){
          int id=connected_devices[i].getId();
          string mac=connected_devices[i].getMAC();
          mac_table[id]=mac;
     }

  }
  void transmission(vector<EndDevices> devices,int sender,int reciever){
    cout<<endl;
    cout<<"Transmission Status :"<<endl;
    cout<<endl;
    bool token=devices[sender-1].token;
    string data=devices[sender-1].SendData();
    if(token==true){
      cout<<data<<" sent successfully from device with MAC "<<mac_table[sender]<< " to "<<mac_table[reciever]<<endl;
    }
  }
  //send ack to sender
  void sendAck(int sender){
   bool ack=connected_devices[sender-1].ack;
   if(ack==true){
   cout<<"ACK was successfully recieved by sender with MAC Address "<<mac_table[sender]<<endl;
   }
   else{
    cout<<"ACK not recieved by sender"<<endl;
   }
  
  }
  
};