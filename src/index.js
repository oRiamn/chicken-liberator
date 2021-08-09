import { getOutpoutPins, setOutPinState } from "@/services/gpio";

import "@/styles/index.scss";
import {
  emitErrorNotification,
  emitSuccessNotification,
  notificationSubject,
} from "./services/notification";

(async () => {
  Vue.component("output-pin", {
    props: ["pin"],
    template: /*html*/ `
      <li>
        <input 
          type="checkbox"
          v-bind:id="pin.pin"
          v-bind:checked="pin.state"
          v-on:change="togglePin"
        >
        <label v-bind:for="pin.pin">
          <span>Pin {{pin.pin}}</span>
          <span></span>
        </label>
      </li>
    `,
    methods: {
      togglePin: async function (evnt) {
        evnt.target.disabled = true;
        try {
          const pin = await setOutPinState(
            this.pin.pin,
            this.pin.state ? 0 : 1
          );
          this.pin.state = pin.state;
          evnt.target.disabled = false;
        } catch (e) {
          emitErrorNotification(
            "Quelque chose n'a pas fonctionné",
            "Impossible de changer l'état de cette sortie",
            e
          );
          setTimeout(() => {
            evnt.target.checked = !evnt.target.checked;
            evnt.target.disabled = false;
          }, 500);
        }
      },
    },
  });

  Vue.component("notification-toaster", {
    data: () => ({
      loaded: false,
      notification: {
        class: "notification",
        title: "",
        message: "",
        type: "",
      },
    }),
    props: [],
    template: /*html*/ `
    <div v-if="loaded" v-bind:class="notification.state">
      <div v-bind:class="notification.type">
        <div class="content">
          <div class="icon">
            <i v-bind:class="notification.class"></i>
          </div>
          <div class="details">
            <span>{{notification.title}}</span>
            <p>{{notification.message}}</p>
          </div>
        </div>
        <div class="close-icon" v-on:click="close">
          <i class="uil uil-times"></i>
        </div>
      </div>
    </div>
    `,
    created: function () {
      notificationSubject.subscribe((e) => {
        this.loaded = true;
        this.notification = e;
        const id = e.id;
        setTimeout(() => {
          if (this.notification.id === id) {
            this.close();
          }
        }, 3000);
      });
    },
    methods: {
      close: function () {
        this.notification.state = "notification hide";
      },
    },
  });

  const app = new Vue({
    el: "#root",
    template: /*html*/ `
    <div id="center">
      <notification-toaster></notification-toaster>
      <h1>Output Pin</h1>
      <button v-on:click="loadPins" v-bind:class="loadingbtn">
        <span class="button__text">Load Pins</span>
      </button>
      <ol class="switches">
        <output-pin
          v-for="item in outputPins"
          v-bind:pin="item"
          v-bind:key="item.pin"
        ></output-pin>
      </ol>
    </div>
    `,
    created: async function () {
      try {
        const outputPins = await getOutpoutPins();
        this.outputPins = outputPins;
        emitSuccessNotification(
          "Chargement terminé",
          "Les pins sont toutes bien là"
        );
      } catch (e) {
        emitErrorNotification(
          "Quelque chose n'a pas fonctionné",
          "Impossible charger les sorties",
          e
        );
      }
    },
    data: {
      outputPins: [],
      loadingbtn: "button",
    },
    methods: {
      loadPins: async function (evnt) {
        this.loadingbtn = "button button--loading";
        try {
          this.outputPins = await getOutpoutPins();
          emitSuccessNotification(
            "Chargement terminé",
            "Les pins sont toutes bien là"
          );
        } catch (e) {
          emitErrorNotification(
            "Quelque chose n'a pas fonctionné",
            "Impossible charger les sorties",
            e
          );
        }
        this.loadingbtn = "button";
      },
    },
  });
})();
