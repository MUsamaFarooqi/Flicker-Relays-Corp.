#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">

<head>

    <title>FAULT Identification Using NodeMCU</title>
    <style>
      body{
    background-image: url("https://images.unsplash.com/photo-1534224039826-c7a0eda0e6b3?ixlib=rb-1.2.1&ixid=MnwxMjA3fDB8MHxzZWFyY2h8MXx8ZWxlY3RyaWNpdHl8ZW58MHx8MHx8&w=1000&q=80")
}
.headDiv{
    /* background-image: url(header.jpg); */
    background-color: black;
    border-radius: 2px;
    /* align-items: center;*/
    justify-content: space-between; 
    /* align-content: center; */
    flex-direction: row;
    display: flex;
    border: 1px solid;
    box-shadow: 1px 4px 10px #888888;

}
.mainHeadText{
    font-size: 60px;
    /* text-align: center; */
    color: white;
    font-weight: bold;
      text-shadow: 2px 2px #c98484;
      align-self: center;        
}
.formDiv{
    background-image: url("https://wallpaperaccess.com/full/818241.jpg");    margin-left: 20%;
    margin-right: 20%;
    padding-top: 10px;
    margin-bottom: 3%;
    margin-top: 2%;
    border: 1px solid;
    padding: 10px;
    box-shadow: 5px 10px 18px #888888;
}

.inputDiv{
    flex-direction: row;display: flex;padding-left: 5%;
    margin-top: 20px;
}

.input{
    width: 250px;padding-top: 3px;padding-bottom: 3px;margin-right: 10px;
}

.headText{
    margin-top: 8px;margin-right: 15px;font-size: 18px;
    margin-left: 10px;
}

.button {
    background-color: #4CAF50;
    border: none;
    color: white;
    padding: 8px 16px;
    text-align: center;
    text-decoration: none;
    display: inline-block;
    font-size: 16px;
    margin: 4px 2px;
    cursor: pointer;
    border-radius: 20px;
    margin-left: 40%;
    margin-top: 2%;
    margin-bottom: 2%;
}
    </style>
    
    

</head>

<body>
     <div class="headDiv">
        <img src="https://media0.giphy.com/media/l3vR16pONsV8cKkWk/giphy.gif" style="width: 220px;height: 150px;"/>
        <marquee width="60%" direction="left" height="80px" class="mainHeadText"  scrollamount="15">
            Flicker Relays           
        </marquee>
        <img src="https://media0.giphy.com/media/l3vR16pONsV8cKkWk/giphy.gif" style="width: 220px;height: 150px;"/>

        <!-- <p class="mainHeadText">Flicker Relays</p> -->
    </div>
    <form action="/action_page">
        <div class="formDiv">
                <div class="inputDiv">
                    <p class="headText">Enter your Circuit Breaker Rating:</p>
             
                    <input type="text" name="CB_Rating" placeholder="CB rating in Amps" size="15" maxlength="30" required="required"
                    class="input" />
                </div>
                <div class="inputDiv">
                    <p class="headText">Enter System Voltage:</p>
             
                    <input type="text" name="sys_vol" placeholder="Voltage in Volts" size="15" maxlength="30" required="required"
                    class="input" />
                </div>
                <div class="inputDiv">
                    <p class="headText">Enter System Frequency:</p>
             
                    <input type="text" name="freq" placeholder="Frequency in Hz" size="15" maxlength="30" required="required"
                    class="input" />
                </div>
                <p class="headText" style="font-weight: bold;">Phase Currents:</p>
                <div class="inputDiv">                 
                    <p class="headText">Phase A:</p>
                    <input type="text" name="mag_A" placeholder = "Magnitude" class="input">
                    <input type="text" name="angle_A" placeholder = "Angle in Degrees" class="input">
                </div>
                <div class="inputDiv">                 
                    <p class="headText">Phase B:</p>
                    <input type="text" name="mag_B" placeholder = "Magnitude" class="input">
                    <input type="text" name="angle_B" placeholder = "Angle in Degrees" class="input">
                </div>
                <div class="inputDiv">                 
                    <p class="headText">Phase C:</p>
                    <input type="text" name="mag_C" placeholder = "Magnitude" class="input">
                    <input type="text" name="angle_C" placeholder = "Angle in Degrees" class="input">
                </div>
                <p class="headText" style="font-weight: bold;">Phase Voltage:</p>
                <div class="inputDiv">                 
                    <p class="headText">Phase A:</p>
                    <input type="text" name="magv_A" placeholder = "Magnitude" class="input">
                    <input type="text" name="anglev_A" placeholder = "Angle in Degrees" class="input">
                </div>
                <div class="inputDiv">                 
                    <p class="headText">Phase B:</p>
                    <input type="text" name="magv_B" placeholder = "Magnitude" class="input">
                    <input type="text" name="anglev_B" placeholder = "Angle in Degrees" class="input">
                </div>
                <div class="inputDiv">                 
                    <p class="headText">Phase C:</p>
                    <input type="text" name="magv_C" placeholder = "Magnitude" class="input">
                    <input type="text" name="anglev_C" placeholder = "Angle in Degrees" class="input">                    
                </div>
                <input type="submit" class="button" value="Submit">               
        </div>
    </form> 
</body>

</html>
)=====";

//SSID and Password of your WiFi router
const char* ssid = "Gr@c€FuL.U$M";
const char* password = "EE18158_NED";

ESP8266WebServer server(80); //Server on port 80, default port assigned for HTTP protocol. 
                            //Send And receives data (or HTML pages) to web server.

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}
//===============================================================
// This routine is executed when you press submit
//===============================================================

boolean isNumeric(String str){
    unsigned int stringLength = str.length();
 
    boolean seenDecimal = false;
    boolean seenSign = false;
 
    for(unsigned int i = 0; i < stringLength; ++i) {
        if (isDigit(str.charAt(i))) {
            continue;
        }
 
        if (str.charAt(i) == '.') {
            if (seenDecimal) {
                return false;
            }
            seenDecimal = true;
            continue;
        }
        if (str.charAt(i) == '-') {
            if (seenSign) {
                return false;
            }
            seenSign = true;
            continue;
        }
        return false;
    }
    return true;
  }

void handleForm() {

   String MagA = server.arg("mag_A"); 
   String AngleA = server.arg("angle_A"); 
   String MagB = server.arg("mag_B"); 
   String AngleB = server.arg("angle_B"); 
   String MagC = server.arg("mag_C"); 
   String AngleC = server.arg("angle_C");
   String MagvA = server.arg("magv_A"); 
   String AnglevA = server.arg("anglev_A"); 
   String MagvB = server.arg("magv_B"); 
   String AnglevB = server.arg("anglev_B"); 
   String MagvC = server.arg("magv_C"); 
   String AnglevC = server.arg("anglev_C");  
   String CB_R = server.arg("CB_Rating");
   String Sys_vol = server.arg("sys_vol");
   String Freq = server.arg("freq");

   double CBR = CB_R.toFloat();
   
   double V1m = MagvA.toFloat();
   double V2m = MagvB.toFloat();
   double V3m = MagvC.toFloat();
   double V1a = AnglevA.toFloat();
   double V2a = AnglevB.toFloat();
   double V3a = AnglevC.toFloat();

   if(isNumeric(CB_R) && isNumeric(MagA) && isNumeric(AngleA) && isNumeric(MagB) && isNumeric(AngleB) && isNumeric(MagC) && isNumeric(AngleC) && isNumeric(MagvA) && isNumeric(AnglevA) && isNumeric(MagvB) && isNumeric(AnglevB) && isNumeric(MagvC) && isNumeric(AnglevC)){

        double Real_Ia = MagA.toFloat() * cos(AngleA.toFloat() * 0.01745329252);
        double Real_Ib = MagB.toFloat() * cos(AngleB.toFloat() * 0.01745329252);
        double Real_Ic = MagC.toFloat() * cos(AngleC.toFloat() * 0.01745329252);
        double Real_aIb = MagB.toFloat() * cos((AngleB.toFloat()+120) * 0.01745329252);
        double Real_a2Ib = MagB.toFloat() * cos((AngleB.toFloat()+240) * 0.01745329252);
        double Real_aIc = MagC.toFloat() * cos((AngleC.toFloat()+120) * 0.01745329252);
        double Real_a2Ic = MagC.toFloat() * cos((AngleC.toFloat()+240) * 0.01745329252);
        
        double Img_Ia = MagA.toFloat() * sin(AngleA.toFloat() * 0.01745329252);
        double Img_Ib = MagB.toFloat() * sin(AngleB.toFloat() * 0.01745329252);
        double Img_Ic = MagC.toFloat() * sin(AngleC.toFloat() * 0.01745329252);
        double Img_aIb = MagB.toFloat() * sin((AngleB.toFloat()+120) * 0.01745329252);
        double Img_a2Ib = MagB.toFloat() * sin((AngleB.toFloat()+240) * 0.01745329252);
        double Img_aIc = MagC.toFloat() * sin((AngleC.toFloat()+120) * 0.01745329252);
        double Img_a2Ic = MagC.toFloat() * sin((AngleC.toFloat()+240) * 0.01745329252);

        double Real_Io = (Real_Ia + Real_Ib + Real_Ic)/3;
        double Real_I1 = (Real_Ia + Real_aIb + Real_a2Ic)/3;
        double Real_I2 = (Real_Ia + Real_a2Ib + Real_aIc)/3;
        
        double Img_Io = (Img_Ia + Img_Ib + Img_Ic)/3;
        double Img_I1 = (Img_Ia + Img_aIb + Img_a2Ic)/3;
        double Img_I2 = (Img_Ia + Img_a2Ib + Img_aIc)/3;
        
        double magIa = sqrt((Real_Ia)*(Real_Ia)+(Img_Ia)*(Img_Ia));
        double magIb = sqrt((Real_Ib)*(Real_Ib)+(Img_Ib)*(Img_Ib));
        double magIc = sqrt((Real_Ic)*(Real_Ic)+(Img_Ic)*(Img_Ic));
        
        double IFmin = 1500;
        String Fault_Type;
        String PhaseA_V;
        String PhaseB_V;
        String PhaseC_V;
        String Phase_Balance;
        String Freq_Sx;

        if(magIa <= (1.25*CBR) && magIb <= (1.25*CBR) && magIc <= (1.25*CBR)){
          Fault_Type = "Normal Condition";
          }
        else if(magIa < (IFmin) && magIb < (IFmin) && magIc < (IFmin)){
          Fault_Type = "Over Load Condition";
          } 
        else{
        
            if((abs(Real_Io - 0)) > 0.1 && (abs(Img_Io - 0)) > 0.1 ){
              if((abs(Real_I1 - Real_I2)) < 0.1 && (abs(Img_I1 - Img_I2)) < 0.1){
                Fault_Type = "Single Line to Ground Fault";
              }
              else{
              Fault_Type = "Double Line to Ground Fault";
            }
         }

            else{
              if((abs(Real_I2 - 0)) < 0.1 && (abs(Img_I2 - 0)) < 0.1){
                Fault_Type = "3 Phase Fault";
              }
              else{
                Fault_Type = "Line to Line Fault";
               }
             }
        }

double sys_vol = Sys_vol.toFloat() ; 
        
       //Voltage Over/Under protection
       //Phase 1
       
       if ((V1m/sys_vol)>0.9){
        if ((V1m/sys_vol)<1.1){
          PhaseA_V = "Phase A Voltage is Normal";
        }
        else{
          PhaseA_V = "Phase A Over Voltage";
            }
       }
       else{
        PhaseA_V = "Phase A Under Voltage";
       }
  
      //Phase 2
      if ((V2m/sys_vol)>0.9){
        if ((V2m/sys_vol)<1.1){
          PhaseB_V = "Phase B Voltage is Normal";
        }
        else{
          PhaseB_V = "Phase B Over Voltage";
          }
       }
       else{
        PhaseB_V = "Phase B Under Voltage";
       }
       
       //Phase 3 
      if ((V3m/sys_vol)>0.9){
        if ((V3m/sys_vol)<1.1){
          PhaseC_V = "Phase C Voltage is Normal";
        }
        else{
          PhaseC_V = "Phase C Over voltage";
        }
       }
       else{
        PhaseC_V = "Phase C Under Voltage";
       }
       
     //Phase Unbalancing Protection

       if((abs(V1m-V2m)/sys_vol<0.1) &&  (abs(V1m-V3m)/sys_vol<0.1)){
      if((abs(V3a-V1a)/(120)>0.9 || abs(V3a-V1a)/(240)>0.95) && (abs(V3a-V1a)/(120)<1.1 || abs(V1a-V3a)/(240)<1.05)){
        if((abs(V2a-V3a)/(120)>0.9 || abs(V2a-V3a)/(240)>0.95) && (abs(V2a-V3a)/(120)<1.1 || abs(V3a-V2a)/(240)<1.05)){
           Phase_Balance = "  Phases are balanced";
        }
        else{
          Phase_Balance = "  Phase assymetry trip  ";
            }
            }
            else{
               Phase_Balance = "  Phase assymetry trip";}
     }
     else{
       Phase_Balance = "  Phase assymetry trip";}
      
      
    double Sf = Freq.toFloat();
    if ((Sf/50)>0.9795){
      if ((Sf/50)<1.025){
        Freq_Sx = "Frequency is Normal.";
        
      }
      else{
        Freq_Sx = "Over Frequency.";
      }
    }
    else{
      Freq_Sx = "Under Frequency.";
    }

      
        
            String html = "<a href='/'> Go Back </a>";
            

html +="<h2>RESULTS</h2> <table> <tr> <td class=\"table\">System Status:</td> <td class=\"table\" id=\"sx\">";
html += Fault_Type;
html +="</td> </tr> </table> <h2>OVER/UNDER VOLTAGES</h2> <table> <tr> <td class=\"table\">Phase A:</td> <td class=\"table\" id=\"sx\">";
html += PhaseA_V;
html +="</td> </tr> <tr> <td class=\"table\">Phase B:</td> <td class=\"table\" id=\"sx\">";
html += PhaseB_V;
html +="</td> </tr> <tr> <td class=\"table\">Phase C:</td> <td class=\"table\" id=\"sx\">";
html += PhaseC_V;
html +="</td> </tr> </table> <h2>ASSYMETRY</h2> <table> <tr> <td class=\"table\">Assymetry Status:</td> <td class=\"table\" id=\"sx\">";
html += Phase_Balance;
html +="</td> </tr> </table> <h2>FREQUENCY</h2> <table> <tr> <td class=\"table\">Frequency Status:</td> <td class=\"table\" id=\"sx\">";
html += Freq_Sx;
html +="</td> </tr> </table> <h2>SEQUENCE COMPONENTS</h2> <table> <tr> <td class=\"table\">Zero Sequence:</td> <td class=\"table\" id=\"sx\">";
html += String(Real_Io);
html +=", ";
html += String(Img_Io);
html +=" i</td> </tr> <tr> <td class=\"table\">Pos Sequence:</td> <td class=\"table\" id=\"sx\">";
html += String(Real_I1);
html +=", ";
html += String(Img_I1);
html +=" i</td> </tr> <tr> <td class=\"table\">Neg Sequence:</td> <td class=\"table\" id=\"sx\">";
html += String(Real_I2);
html +=", ";
html += String(Img_I2);
html +=" i</td> </tr> </table>";
html += "<style> h2 { border-bottom: 2px solid #4d004d; } .table { font-weight: bold; font-size: 20px; } #sx { color: #0000e6; } </style>";

     server.send(200, "text/html", html); //Send web page
        
  }
  else{
                String s = "<a href='/'> Go Back </a>";
     s += "<h2>Please Enter Numeric Input!</h2>";   
     server.send(200, "text/html", s); //Send web page
    }
}
//==============================================================
//                  SETUP
//==============================================================
void setup(void){
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println("WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
 
  server.on("/", handleRoot);      //Which routine to handle at root location
  server.on("/action_page", handleForm); //form action is handled here

  server.begin();                  //Start server
  Serial.println("HTTP server started");
}
//==============================================================
//                     LOOP
//==============================================================
void loop(void){
  server.handleClient();          //Handle client requests
}
