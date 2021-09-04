import { setOutPinState, pinSubject } from "@/services/gpio";
import { emitErrorNotification } from "@/services/notification";
import { filter } from "rxjs";

export default Vue.component("OutputPin", {
  props: {
    pin: {
      type: Object,
      default: null,
    },
  },
  data: function () {
    return {
      mutablePin: this.pin,
    };
  },
  created: function () {
    pinSubject.pipe(filter((p) => p.pin === this.pin.pin)).subscribe((p) => {
      this.mutablePin.state = p.state;
    });
  },
  methods: {
    togglePin: async function (evnt) {
      evnt.target.disabled = true;
      try {
        await setOutPinState(this.pin.pin, this.pin.state ? 0 : 1);
        evnt.target.disabled = false;
      } catch (e) {
        emitErrorNotification(
          "Oops!",
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
  template: /* html */ `
      <li>
        <input 
          type="checkbox"
          v-bind:id="pin.pin"
          v-bind:checked="mutablePin.state"
          v-on:change="togglePin"
        >
        <label v-bind:for="pin.pin">
          <span>Pin {{pin.pin}}</span>
          <span></span>
        </label>
      </li>
    `,
});
