import { Subject } from "rxjs";

export const notificationSubject = new Subject();

export const emitErrorNotification = (title, message, e) => {
  console.error(e);
  notificationSubject.next({
    class: "uil uil-times",
    state: "wrapper active",
    type: "toast error",
    title,
    message,
  });
};

export const emitSuccessNotification = (title, message) => {
  notificationSubject.next({
    class: "uil uil-check",
    state: "wrapper active",
    type: "toast success",
    title,
    message,
  });
};
