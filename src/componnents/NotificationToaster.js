import { notificationSubject } from "@/services/notification";

export default Vue.component("NotificationToaster", {
  props: [],
  data: () => ({
    loaded: false,
    notification: {
      class: "notification",
      title: "",
      message: "",
      type: "",
    },
  }),
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
  template: /* html */ `
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
});
