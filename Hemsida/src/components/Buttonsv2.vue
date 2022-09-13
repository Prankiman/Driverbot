<template>
  <v-container id="box">
  <div id = "joyContainer">
    <div id = "joyStick"></div>
  </div>
  <span id="hide">{{Disconnect}}</span>
      <v-btn v-if="!connected" id = "connectButton" class="ma-2" tile large :color="car" @click="Connect()">
        <span>Connect</span>
      </v-btn>
  </v-container>
</template>

<script>
//Bibliotek som ska användas
var mqtt = require("mqtt");
export default {
  name: "Buttons",
  props: {
    //Data som skickas in i komponenten
  },
  data() {
    //All data som ska finnas i komponenten
    return {
      connected: false,
      car: "green",
      clientId: "notyetAssigned",
      client: null,
      options: {},
      speed: 500
    };
  },
  created() {
    //När komponenten är skapad
  },
  mounted() {
    //När komponenten är mountad (inladdad)

    let circle = document.getElementById('joyStick');

    let left = 0;
    let top = 0;

    var mouseDown = false;
    
    addEventListener('mousemove', (e) => {
       e.preventDefault();
       if(e.pageX > 300 && e.pageX < 700)
        left = e.pageX;
       if(e.pageY > 400 && e.pageY < 800)
        top = e.pageY;
      if(mouseDown == true){
        if(e.pageY < 400){
          this.Send('drive','b1000');
        }
        if(e.pageY > 800){
          this.Send('drive','f1000');
        }

        if(e.pageX < 300){
          this.Send('drive','r15');
        }
        if(e.pageX > 700){
          this.Send('drive','l15');
        }

       
          circle.style.left = left-410 + 'px';
          circle.style.top = top-510 + 'px';
        }
        else{
           circle.style.left = 100 + 'px';
           circle.style.top = 100 + 'px';
        }
       
    });
    addEventListener('mousedown', (e) => {
        mouseDown = true;
    });
    addEventListener('mouseup', (e) => {
        mouseDown = false;
        this.Send('drive', 'f0');
    });
    console.log(mouseDown);

  },
  computed: {
    Disconnect() {
      if (this.$store.getters.connected == false) {
        // this.Send('drive','f0')
        return true;
      }
      return false;
    }
  },
  watch: {
    Disconnect: {
      handler: function(newVal) {
        if (newVal == true) {
          this.connected = false;
          this.client = mqtt.disconnect;
          this.car = "green";
        }
      }
    }
  },
  methods: {
    //Metoder
    Connect() {
      let ref = this;
      if (this.connected == true) {
        return "";
      }

      let User = this.$store.getters.GetUser;
      this.clientId =
        "DriverControll" +
        Math.random()
          .toString(16)
          .substr(2, 8);
      var mqtt_url = "192.168.1.116";//maqiatto.com"
      var url = "mqtt://" + mqtt_url;
      var options = {
        port: User.port,
        clientId: this.clientId,
        username: User.name,
        password: User.password
      };

      this.options = options;
      // console.log("connecting");
      this.client = mqtt.connect(url, options);
      // console.log("connected?");

      this.connected = true;
      this.client
        .on("connect", function() {
          // console.log("success");
          ref.Connecting(true);
        })
        .on("error", function() {
          // console.log("error");
          ref.Connecting(false);
        })
        .on("close", function() {
          ref.Connecting(false);
          // console.log("closing");
        });

      this.$store.dispatch("Connect", this.connected);

      if (this.connected == false) {
        this.car = "red";
      } else {
        this.car = "blue";
        this.Send("drive", this.clientId + " har anslutits.");
      }
    },

    Connecting(connected) {
      this.connected = connected;
      this.$store.dispatch("Connect", connected);
      // console.log(this.connected)
      if (connected == false) {
        this.car = "red";
      } else {
        this.car = "blue";
        this.Send("drive", this.clientId + " har anslutits.");
      }
    },

    Send(adress, message) {
      // console.log(message);
      this.client.publish(
        this.options.username + "/" + adress, //Exempel         "exempel@gmail.com"+"/" + "drive",
        message
      );

      this.$store.dispatch("addToLogger", message);
    }
  }
};
</script>

<!-- CSS -->
<style scoped>
.big {
  font-size: 25px;
}
#Cooltext {
  color: black;
  text-decoration: underline;
}
#box {
  width: 100px;
  height: 100px;
}

#connectButton {
  position: absolute;
  left: 800px;
  top: 500px;
  width: 200px;
  height: 200px;
}

#joyContainer {
  position: absolute;
  left: 300px;
  top: 400px;
  width: 400px;
  height: 400px;
  border: solid 20px;
  border-color: red;
  border-radius: 50%;
 
}

#joyStick {
   position: absolute;
   /* left: 50%;
   top: 50%;
   transform: translate(-50%, -50%); */
   width: 150px;
   height: 150px;
   border-radius: 50%;
   background-color: blue;
}

#logger {
  position: fixed;
  bottom: 0;
  right: 0;
  width: 300px;
  height: 300px;
  border: black dotted 2px;

  word-wrap: break-word;
}
#hide{
 display: none;
}
</style>
