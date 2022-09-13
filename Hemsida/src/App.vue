<template>
  <v-app id="content">
    <v-toolbar app id="toolbar">
      <v-toolbar-title class="headline text-uppercase">
        <span>Robot</span>
        <span class="font-weight-light">Car</span>
      </v-toolbar-title>

      <v-spacer></v-spacer>
      <v-btn color="primary" dark @click.stop="dialog = true">Anslutningsalternativ</v-btn>
    </v-toolbar>

    <v-content>
      <v-row align="center" justify="center">
        <v-dialog v-model="dialog" max-width="290" height="290">
          <v-card>
            <v-card-title class="headline">Anslutning till Server</v-card-title>

            <v-card-text style="height: 300px;">
              <v-text-field v-model="name" label="Name" filled></v-text-field>

              <v-text-field v-model="password" label="MQTT Password" filled></v-text-field>

              <v-text-field v-model="port" label="Port" filled></v-text-field>
              <v-text-field v-model="adress" label="Adress" filled></v-text-field>
            </v-card-text>

            <v-card-actions>
              <v-spacer></v-spacer>

              <v-btn color="green darken-1" text @click="save()">Spara</v-btn>
            </v-card-actions>
          </v-card>
        </v-dialog>

        <div>
          <Logger />
          <Buttons2/>
        </div>
      </v-row>
    </v-content>
  </v-app>
</template>

<script>
import Buttons2 from "./components/Buttonsv2";
import Logger from "./components/Logger";

export default {
  name: "App",
  components: {
    Buttons2,
    Logger
  },
  data() {
    return {
      Switch: false,
      dialog: false,
      name: "",
      password: "",
      port: "",
      adress: ""
    };
  },
  created() {
    let User = this.$store.getters.GetUser;
    this.name = User.name;
    this.password = User.password;
    this.port = User.port;
    this.adress = User.adress;
  },

  methods: {
    save() {
      let User = {
        name: this.name,
        password: this.password,
        port: this.port,
        adress: this.adress
      };

      this.$store.dispatch("Save", User);

      this.dialog = false;
    }
  },
  computed: {}
};
</script>
<style scoped>
.img {
  width: 40%;
  height: 40%;
  margin: auto;
  padding: 10px;
}
#logger {
  /* position: fixed;
  bottom: 0;
  right: 0; */
  width: 300px;
  height: 300px;
  border: black dotted 2px;

  word-wrap: break-word;
}

</style>
