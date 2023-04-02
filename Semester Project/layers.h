#include<iostream>
#include<map>
#include<vector>
using namespace std;
 
class EndDevices{
   private:
   int deviceId;
   string MAC_Address;
   string message;
   
   public:
    bool ack;
    EndDevices() {
        deviceId = 0;
        MAC_Address="";
        ack=false;
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
    void sendData(string data){
        message=data;
       
    }
   string getData(){
    return message;
   }

   void sendAck(int reciever){
     ack=true;
     cout<<"Device "<<reciever<<" sends ACK to hub"<<endl;
     
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
    string data=devices[sender-1].getData();
   
    //hub broadcasts data 
    for(int i=0;i<connected_devices.size();i++){
      if(i!=sender-1){
      connected_devices[i].sendData(data);
      }
    }
    
  }
  //status of transmission
  void transmission(int sender,int reciever){
    for(int i=0;i<connected_devices.size();i++){
       string message=connected_devices[i].getData();
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
  vector<EndDevices> connected_devices;
  public:
   void topology(EndDevices devices){
    //connecting end devices to switch
    connected_devices.push_back(devices);
   } 
  void print_connection(int i){
    cout<<"Connection successfully created between switch and device with MAC_Address: "<<connected_devices[i].getMAC()<<endl;
  }

};