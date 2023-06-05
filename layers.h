#include<iostream>
#include<cstdlib>
#include<unistd.h>
#include<chrono>
#include<map>
#include<vector>
#include <random>
#include <bitset>
#include <algorithm>
using namespace std;

// Forward declaration of the Switch class
class Switch;

class EndDevices{
   private:
   int deviceId;
   string MAC_Address;
   string IP_Address;
   string message;
   public:
    map<string,string> arp;
    map<int,bool> selective_window;
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
    EndDevices(int Id,string mac,string ip){
        deviceId=Id;
        MAC_Address=mac;
        IP_Address=ip;

    }
    int getId(){
        return deviceId;
    }
    string getMAC(){
        return MAC_Address;
    }
    string getIP(){
        return IP_Address;
    }
    void getData(string data){
        message=data;
        
       
    }
   string SendData(){
    return message;
   }

   void sendAck(int reciever){
     ack=true;
     cout<<endl;
     cout<<"Acknowledgement Status: "<<endl;
     cout<<endl;
     cout<<"Device "<<reciever<<" sends back ACK to Hub "<<endl;
     cout<<endl;
     
   }
   void tokenCheck(vector<EndDevices> &devices,int sender,int size){
      
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
    void sender(vector<int> window){
      cout<<endl;
       //window sliding 
       int i=0;
       while(i<window.size()){
           srand(time(0));
           ack=false;
           int timeout_duration=4,sending_time=rand()%6,recieving_time=rand()%6; 
           sender_buffer=window[i];
           //sending packet to reciever
           int AckNo;
           sleep(sending_time); 
           if(sending_time> timeout_duration){         //packet got lost
            cout<<"Sender sends packet with sequence number "<<window[i] <<" but it got lost"<<endl; 
            continue;          //resend packet
           }
           else{
            cout<<"Sender sends packet with sequence number "<<sender_buffer<<endl;
            AckNo=reciever(window,i);
            //reciever reaches at the end of window
              if(AckNo==-1){
                cout<<"Done"<<endl;
                break;
              }
            if(recieving_time>timeout_duration){      //ACK got lost
               cout<<"ACK "<<AckNo<< " got lost"<<endl;
               continue;           //resend packet
            }
            else{
              if(ack==true){
              cout<<"Sender recieves ACK "<<AckNo<<endl;  //ACK Recieved
              i++;
              }
            }
           } 
           
       }
       
    }
    int j=0;
    int reciever(vector<int> window,int i){
       
        
        if(sender_buffer==window[j] && i==j && j<window.size()){
        reciever_buffer=sender_buffer;
         ack=true;
         j++;
         if(j==window.size()){
          return -1;
         }
        return window[j];
         
        }
        else{
           cout<<"Packet "<<window[i]<<" was discarded as it a duplicate "<<endl;       //ACK Lost Case
           ack=true;
           return window[j];
        }

    }
    //Flow control Protocol
      void StopAndWait(){
        int windowSize=7;
        vector<int> window;
        for(int i=0;i<windowSize;i++){
          if(i%2==0){
            window.push_back(0);
          }
          else{
            window.push_back(1);
          }
        }
        cout<<endl;
        cout<<"Transmission Status :"<<endl;
        sender(window);
           
       }
      int R_n=0;
      int selective_reciever(int packet){
        
        selective_window[packet]=true;
        int AckNo=packet;
        //slide recieving window if consecutive elements are marked
        int count=0;
          for(int j=0;j<selective_window.size();j++){
                if(selective_window[j]==false){
                  break;
                }
                count++;
          }
          R_n=count;
          ack=true;
        return AckNo;

      }
    void selective_sender(){
           int S_n=0,S_f=1,S_z=selective_window.size();
           int i=0,AckNo;   
           while(i<S_z){
           srand(time(NULL));
           ack=false;
           int timeout_duration=4,sending_time=rand()%6,recieving_time=rand()%6; 
          
          
           sleep(sending_time); 
           if(sending_time> timeout_duration){         //packet got lost
            cout<<"Sender sends packet with sequence number "<<i <<" but it got lost"<<endl; 
            i++;
            continue;
           }
           else{
            int packet=i;
            cout<<"Sender sends packet with sequence number "<<packet<<endl;
            AckNo=selective_reciever(packet);
           
            if(recieving_time>timeout_duration){      //ACK got lost
               cout<<"ACK "<<AckNo<< " got lost"<<endl;
               i++;
            }
            else{
              if(ack==true){
                cout<<"ACK "<<AckNo<<" recieved"<<endl;
                int count=0;
                //slide window if consecutive elemnts in window are marked
              for(int j=0;j<=AckNo;j++){
                if(selective_window[j]==false){
                  break;
                }
                count++;
              }
              S_f=count;
              
              i++;
              S_n=i;
              }
            }
            
           }
           
          
         } 
         //timeout
          if(i==S_z){
            cout<<endl;
             cout<<"Time out occured"<<endl;
            
             //check which packet  is not recieved and resend it
             for(int j=0;j<selective_window.size();j++){
               
              if(selective_window[j]==false){
                cout<<"Resending Packet "<<j<<" as it wasn't recieved"<<endl;
                //resending packet
                AckNo=selective_reciever(j);
                cout<<"ACK "<<AckNo<<" recieved"<<endl;
              }
             }
           }
    }
   //Selective Repeat protocol
   void Selective_Repeat(){
    //m=4
    cout<<endl;
    int size=8;
    for(int i=0;i<size;i++){
      selective_window[i]=false;
    }
    selective_sender();

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
   //arp cache
   void arp_cache(string ip,string mac){
        arp[ip]=mac;      
        
   }
   void print_ArpCache(){
        cout<<endl;
        cout<<"ARP Cache of sender is as :"<<endl;
        for(auto it: arp){
          cout<<"IP : "<<it.first<<" -> "<<"MAC: "<<it.second<<endl;
        }
   }
};

class hub{
  private:
  int hub_ID;
  vector<EndDevices> connected_devices;        //vector for storing endDevice objects in hub
  public:
  bool ack;
  string data;
  hub() {
        hub_ID= 0;
        ack=false;
    }
  hub(int Id){
        hub_ID=Id;
        
  }
  int getId(){
        return hub_ID;
   }
   //return devices connected to hub
  vector<EndDevices> getDevices(){
    return connected_devices;
  }
  void topology(EndDevices &devices){
    //connecting end devices to hub
    connected_devices.push_back(devices);
    
  }
  void connection(int i){
    for(int j=0;j<connected_devices.size();j++){
      cout<<"Connection Established between hub "<<i<<" and End device with ID "<<connected_devices[j].getId()<<endl;
    }
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
      // if(i!=sender-1){
      connected_devices[i].getData(data);
      // }
    }
    
  }
  
  //status of transmission
  void transmission(int sender,int reciever){
    cout<<endl;
    cout<<"Transmission status: "<<endl;
    cout<<endl;
    for(int i=0;i<connected_devices.size();i++){
       string message=connected_devices[i].SendData();
       int Current_device=connected_devices[i].getId();
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
     cout<<"Hub Broadcasts ACK"<<endl;
     cout<<endl;
     if(connected_devices[reciever-1].ack==true){
      for(int i=0;i<connected_devices.size();i++){
       int Current_device=connected_devices[i].getId();
       if(Current_device!=reciever){
       if(Current_device!=sender ){
         cout<<"ACK was recieved by device "<<Current_device<<" but it was discarded"<<endl;
       }
       else{
        cout<<"ACK was recieved by device "<<Current_device<<" and it was accepted"<<endl;
       }
      }
      }
     }
  }
};


class Router: public EndDevices{
 public:
  string IP1,IP2,MAC1,MAC2;
  vector<Switch> connected_devices;
  //intialising ip and mac of interfaces of router
  void setAddress(string IP1,string IP2,string  MAC1,string  MAC2){
    this->IP1=IP1;
    this->IP2=IP2;
    this->MAC1=MAC1;
    this->MAC2=MAC2;
  }
  //connect switch to router
  void ConnectSwitch(Switch& s){
    connected_devices.push_back(s);
  }
  //generate NID 
  
int random(int min, int max) {
 
  return rand() % (max - min + 1) + min;
}

// function to generate a random NID
    string generate_NID() {
      string NID = "";
      // Generate four octets randomly between 0 and 255
      for (int i = 0; i < 4; i++) {
        int octet = random(0, 255);
        // Append the octet to the IPv4 address with a dot
        if(i<2){
        NID += to_string(octet) + ".";
        }
        else{
          NID+=to_string(0)+".";
        }
      }
      // Remove the last dot
      NID.pop_back();
      return NID;
}
//generate classless IPV4 Address
string generate_classless_ip(string NID){
  NID.pop_back();
  
    for (int i = 0; i < 4; i++) {
        int octet = random(0, 255);
        // Append the octet to the IPv4 address with a dot
        
        if(i==3){
          NID+=to_string(octet)+"/24";
        }
      }
      
      return NID;
}
bool sameNID(string sourceIp,string destinationIp){
  for(int i=0;i<6;i++){
    if(sourceIp[i]!=destinationIp[i]){
      return false;
    }
  }
  return true;
}
int NetworkNo(string sourceIp){
  if(IP1.substr(0,6)==sourceIp.substr(0,6)){
    return 1;
  }
  else{
    return 2;
  }
}
};

class Switch :public EndDevices{
  private:
  int switchId;
  map<int, vector<int>> hub_DeviceMap;
  map<int,string> mac_table;
  vector<EndDevices*> connected_devices;
  vector<hub> connected_hubs;
  string data;
  public:
   Switch(){
    data="";
   }
   Switch(string message){
     data=message;
   }
   void topology(EndDevices &devices){
    //connecting end devices to switch
    connected_devices.push_back(&devices);
   } 
  void print_connection(int i){
    cout<<"Connection successfully established between switch and device with MAC_Address: "<<connected_devices[i]->getMAC()<<endl;
  }
  void topology(hub &hubs){
    //connecting hubs to switch
    connected_hubs.push_back(hubs);
  }
  void hub_print_connection(int i){
    cout<<"Connection successfully established between switch and hub with Hub ID : "<<connected_hubs[i].getId()<<endl;
  }
  void HubToDeviceMap(int hubId,vector<EndDevices> &devices){
       vector<int> devices_id;
      for(int i=0;i<devices.size();i++){
        int id=devices[i].getId();
        devices_id.push_back(id);
      }
      //mapping between hub and end devices connected to it
      hub_DeviceMap[hubId]=devices_id;

  }
  void print_HubToDeviceMap(){
    cout<<"Mapping of Hub and End devices, stored in switch"<<endl;
    cout<<endl;
    for(auto it:hub_DeviceMap){
        int hubId = it.first+1;
        vector<int> deviceIds = it.second;
        cout << "End devices ";
        for (int i = 0; i < deviceIds.size(); i++) {
            cout << deviceIds[i];
            if (i < deviceIds.size() - 1) {
                cout << ",";
            }
        }
        cout << " are connected to hub " << hubId << endl;
        
         
    }
    cout<<endl;
  }
  int findHubForDevice(int deviceId) {
    for (auto iter = hub_DeviceMap.begin(); iter != hub_DeviceMap.end(); ++iter) {
        int hubId = iter->first;
        vector<int> deviceIds = iter->second;
        if (find(deviceIds.begin(), deviceIds.end(), deviceId) != deviceIds.end()) {
            return hubId;
        }
    }
    // Device not found in any hub
    return -1;
}

  void MAC_table(){
     
     //switch storing mapping of device id and MAC address
     for(int i=0;i<connected_devices.size();i++){
          int id=connected_devices[i]->getId();
          string mac=connected_devices[i]->getMAC();
          mac_table[id]=mac;
     }

  }
  int recieveData(int sender,int reciever,string message){
    // get data from sorce hub
    data=message;
    int source_hub=findHubForDevice(sender);
    int destination_hub=findHubForDevice(reciever);
    cout<<"Switch recieved "<<message<<" from hub "<<source_hub+1<<endl;
    //send data to destination hub
    connected_hubs[destination_hub].data=message;
    cout<<"Switch sends "<<message<<" to hub "<<destination_hub+1<<endl;
    return destination_hub;
  }
  void transmission(vector<EndDevices> &devices,int sender,int reciever){
    cout<<endl;
    cout<<"Transmission Status :"<<endl;
    cout<<endl;
    
    bool token=connected_devices[sender-1]->token;
    string data=devices[sender-1].SendData();
      if(token==true){
      cout<<data<<" sent successfully from device with MAC "<<mac_table[sender]<< " to "<<mac_table[reciever]<<" via  switch"<<endl;
      }
  }
  //send ack to sender in case of end devices
  void sendAck(int sender){
   bool ack=connected_devices[sender-1]->ack;
   if(ack==true){
   cout<<"ACK was successfully recieved by sender with MAC Address "<<mac_table[sender]<<endl;
   }
   else{
    cout<<"ACK not recieved by sender"<<endl;
   }
  
  } 
  //recieve ack from destination hub
  void recieveAck(int destination_hub){
   if(connected_hubs[destination_hub].ack==true){
    cout<<"Hub "<<destination_hub+1<<" sends ACK to switch"<<endl;
   }
  }
  //send ACK to source hub
  void send_Ack(int source_hub){
      connected_hubs[source_hub].ack=true;
      cout<<"Switch sends ACK to Hub "<<source_hub+1<<endl; 
  }

  //broadcast arp request
  string broadcast_Arp(string destinationIp,Router &r,int network){
    cout<<"Switch broadcast ARP request"<<endl;
    cout<<endl;
    cout<<"Who is "<<destinationIp<<" ?"<<endl;
    cout<<endl;
     for(int i=0;i<connected_devices.size();i++){
      string result=connected_devices[i]->arp[destinationIp];
      if(result.length()!=0){
         cout<<"ARP Reply : Source IP : "<<connected_devices[i]->getIP()<<" Source MAC : "<<connected_devices[i]->getMAC()<<endl;
       return connected_devices[i]->getMAC();
      }
      
     }
     //return MAC address of default gateway
     if(network==1){
     cout<<"ARP Reply by Default Gateway : Source IP : "<<r.IP1<<" Source MAC : "<<r.MAC1<<endl;
     return r.MAC1;
     }
     else{
      cout<<"ARP Reply by Default Gateway : Source IP : "<<r.IP2<<" Source MAC : "<<r.MAC2<<endl;
     return r.MAC2;
     }
  
  
  }
   //send message
   void sendMessage(EndDevices & devices ,string destinationIP){
           cout<<endl;
           string destination_mac=devices.arp[destinationIP];
           string source_mac=devices.getMAC();
           cout<<devices.SendData()<<" sent successfully from device with MAC "<<source_mac<<" to "<<destination_mac<<endl;
   }
};

