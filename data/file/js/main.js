let textLanguage = document.querySelector("#idioma-text");
let intro = document.querySelector("#intro-transition");
let body = document.querySelector("body");
let dark_mode = false;
let browser = document.querySelector("#theme-browser");
let theme_button = document.querySelector("#theme-button");
let theme_image = document.querySelector("#theme");
let step = 1;

let display1 = document.querySelector("#first-display");
let display2 = document.querySelector("#second-display");
let display3 = document.querySelector("#third-display");

let next = document.querySelector("#next");
let testButton = document.querySelector("#test");
let testModal = document.querySelector("#testModal");
let testSensorsModal = document.querySelector("#testSensorsModal");
let closeModalTest = document.querySelector("#closeModalTest");
let temperatureSectionButton = document.querySelector("#temperatureSection");
let motionSection = document.querySelector("#motionSection");
let sensorTHpage = document.querySelector("#sensorTHpage");
let sensorPIRpage = document.querySelector("#sensorPIRpage");
let tryButton = document.querySelector("#try");
let motionText = document.querySelector("#motionText");
let motionDetectImage = document.querySelector("#motion-detect");


let loadingPage = document.querySelector("#loadingPage");
let sensorsGraphics = document.querySelectorAll(".sensorData");
let errorSensorPage = document.querySelector("#errorSensorPage");
let temperatureGraphic = document.querySelector("#temperatureGraphic");
let humidityGraphic = document.querySelector("#humidityGraphic");
let websocket;
let websocket2;

const LIGHT_ICON = "light";
const DARK_ICON = "darkImage";
const WHITE = "#FFFFFF";
const DARK = "#000000";
const ROOT_IMG = "file/img/";

theme_button.addEventListener("click", () => {
  theme_image.classList.add("translate");

  setTimeout(() => {
    if (dark_mode) {
      dark_mode = false;
      body.classList.remove("dark");
      theme_image.setAttribute("src", ROOT_IMG + DARK_ICON + ".png");
      browser.setAttribute("content", WHITE);
    } else {
      dark_mode = true;
      body.classList.add("dark");
      theme_image.setAttribute("src", ROOT_IMG + LIGHT_ICON + ".png");
      browser.setAttribute("content", DARK);
    }
    theme_image.classList.remove("translate");
  }, 1000);
});

let darkMode = false;

setTimeout(() => {
  intro.classList.add("hidden");
  browser.setAttribute("content", WHITE);
}, 6500);

let intervalText = setInterval(() => {
  let es = document.querySelector("#idioma-text-es");
  es.classList.toggle("hideDown");
  es.classList.toggle("showUp");

  let en = document.querySelector("#idioma-text-en");
  en.classList.toggle("showUp");
  en.classList.toggle("hideDown");
}, 3000);

temperatureSectionButton.addEventListener("click", () => {
  temperatureSectionButton.classList.add("optionSelected");
  motionSection.classList.remove("optionSelected");
  sensorTHpage.classList.remove("hidden");
  sensorPIRpage.classList.add("hidden");
});

motionSection.addEventListener("click", () => {
  motionSection.classList.add("optionSelected");
  temperatureSectionButton.classList.remove("optionSelected");
  sensorTHpage.classList.add("hidden");
  sensorPIRpage.classList.remove("hidden");
});

document.querySelector("#es").addEventListener("click", () => {
  setLanguage("es");
});

document.querySelector("#en").addEventListener("click", () => {
  setLanguage("en");
});

testButton.addEventListener("click", () => {
  updateSensors();
  testSensorsModal.classList.remove("hidden");
});

function closeWebsocket() {
  websocket.close();
  websocket2.close();
}


tryButton.addEventListener("click", () => {
  updateSensors();
});

closeModalTest.addEventListener("click", () => {
  testModal.classList.add("hiddenAnimation");
  testSensorsModal.classList.add("hiddenBack");
  closeWebsocket();
  
  setTimeout(() => {
    testSensorsModal.classList.remove("hiddenBack");
    testSensorsModal.classList.add("hidden");
    testModal.classList.remove("hiddenAnimation");
  }, 300);
});

function setLanguage(language) {
  display1.classList.add("notShowing");

  setTimeout(() => {
    secondDisplay();
  }, 500);
}

function secondDisplay() {
  display1.classList.add("hidden");
  display2.classList.remove("hidden");
}

function thirdDisplay() {
  display2.classList.add("notShowing");

  setTimeout(() => {
    display2.classList.add("hidden");
    display3.classList.remove("hidden");
  }, 500)
}

function updateSensors() {
  runWebSocket();
}

function showLoading() {
  loadingPage.classList.remove("hidden");
  errorSensorPage.classList.add("hidden");
  sensorsGraphics.forEach((graphic) => {
    graphic.classList.add("hidden");
  });
}

function showSensorsGraphics() {
  sensorsGraphics.forEach((graphic) => {
    graphic.classList.remove("hidden");
  });
  loadingPage.classList.add("hidden");
  errorSensorPage.classList.add("hidden");
}

function showError(error) {
  sensorsGraphics.forEach((graphic) => {
    graphic.classList.add("hidden");
  });
  loadingPage.classList.add("hidden");
  errorSensorPage.classList.remove("hidden");
  document.querySelector("#showErrorSensor").innerText = error;
}

function runWebSocket() {
  
  showLoading();

  websocket = new WebSocket("ws://" + location.hostname + ":81/", [
    "arduino",
  ]);

  websocket2 = new WebSocket("ws://" + location.hostname + ":82/", [
    "arduino",
  ]);

  websocket.onopen = () => {
    showSensorsGraphics();
  };

  websocket.onerror = () => {
    showError("Servidor no disponible");
  };
  websocket.onmessage = (data) => {
    refreshSensorTH(data.data);
  };

  websocket2.onmessage = () => {
    showMotion();
  };
}

function showMotion() {
  motionText.innerText = "¡Movimiento detectado!";
  motionText.classList.add("detectMotion");
  motionDetectImage.classList.add("hiddenPIR");

  setTimeout(() => {
    motionDetectImage.classList.remove("hiddenPIR");
    motionText.classList.remove("detectMotion");
    motionText.innerText = "Detectando movimiento...";
  }, 3000);
}

function refreshSensorTH(response) {
  let responseJson = JSON.parse(response);

  if (responseJson.error != undefined) {
    showError(responseJson.error);
  }
  else {
    showSensorsGraphics();
    temperatureGraphic.style = "--angule: " + (parseInt(responseJson.tempCelcius) * 3.6)  + "deg; --text: '" + responseJson.tempCelcius + " C° " + responseJson.tempFahrenheit + " F'";
    humidityGraphic.style = "--angule: " + (parseInt(responseJson.humidity) * 1.8)  + "deg; --text: '" + responseJson.humidity + " %'";
  }
}


document.querySelector("#next2").addEventListener("click", () => {
  thirdDisplay();
});