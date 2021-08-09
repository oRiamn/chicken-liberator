import { Subject } from "rxjs";
import { v4 as uuidv4 } from "uuid";

export const notificationSubject = new Subject();

const emit = (n) => {
  notificationSubject.next({
    ...n,
    id: uuidv4(),
  });
};

export const emitErrorNotification = (title, message, e) => {
  console.error(e);
  emit({
    class: "uil uil-times",
    state: "notification active",
    type: "toast error",
    title,
    message,
  });
};

export const emitSuccessNotification = (title, message) => {
  emit({
    class: "uil uil-check",
    state: "notification active",
    type: "toast success",
    title,
    message,
  });
};
