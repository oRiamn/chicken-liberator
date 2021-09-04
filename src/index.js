import { getOutpoutPins } from "@/services/gpio";

import "@/componnents/OutputPin";
import "@/componnents/NotificationToaster";
import "@/styles/index.scss";

import {
  emitErrorNotification,
  emitSuccessNotification,
} from "./services/notification";

(async () => {
  const app = new Vue({
    el: "#root",
    data: {
      outputPins: [],
      loadingbtn: "button",
    },
    created: async function () {
      try {
        const outputPins = await getOutpoutPins();
        this.outputPins = outputPins;
        emitSuccessNotification("Fini!", "Les pins sont toutes bien là");
      } catch (e) {
        emitErrorNotification("Oops!", "Impossible charger les sorties", e);
      }
    },
    methods: {
      loadPins: async function () {
        this.loadingbtn = "button button--loading";
        try {
          this.outputPins = await getOutpoutPins();
          emitSuccessNotification("Fini!", "Les pins sont toutes bien là");
        } catch (e) {
          emitErrorNotification("Oops!", "Impossible charger les sorties", e);
        }
        this.loadingbtn = "button";
      },
    },
    template: /* html */ `
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
  });

  return app;
})();
