
// Elementos con funciones

let intro = document.querySelector("#intro-transition");
let body = document.querySelector("body");
let dark_mode = false;
let browser = document.querySelector("#theme-browser");
let theme_button = document.querySelector("#theme-button");
let theme_image = document.querySelector("#theme");
let listOfNetworks = document.querySelector("#networksList");
let passwordWifiInput = document.querySelector("#passwordWifi");
let submitButton = document.querySelector("#connectWifiButton");


// Modales

let wifiPasswordModalBack = document.querySelector("#wifiPasswordModalBack");
let wifiPasswordModal = document.querySelector("#wifiPasswordModal");
let closeWifiModal = document.querySelector("#closeWiFiModal");
let timeout;

// Datos

let ssidSelected;
let password;
let user;
let passwordUser;

// Pantallas

let display1 = document.querySelector("#first-display");
let display2 = document.querySelector("#second-display");
let display3 = document.querySelector("#third-display");
let display4 = document.querySelector("#four-display");


// Botones y secciones

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

// Pantallas y pestañas

let loadingPage = document.querySelector("#loadingPage");
let sensorsGraphics = document.querySelectorAll(".sensorData");
let errorSensorPage = document.querySelector("#errorSensorPage");
let temperatureGraphic = document.querySelector("#temperatureGraphic");
let humidityGraphic = document.querySelector("#humidityGraphic");
let websocket;
let websocket2;

// Constantes

    const LIGHT_ICON = "light";                                             // Tema claro
    const DARK_ICON = "darkImage";                                          // Tema oscuro
    const WHITE = "#FFFFFF";                                                // Fondo tema claro
    const DARK = "#000000";                                                 // Fondo tema oscuro
    const ROOT_IMG = "file/img/";                                           // Directorio de ficheros
    const PRACTISES_MANAGEMENT_URL = "https://github.com/Tequinox-Z/IoT-Practises-Management";                   // Url derl proyecto
    const PROYECT_URL = "https://github.com/Tequinox-Z/IoT-Practises-Management";
    const TIME_NOTIFICATION = 5000;
    const DELAY_NOTIFICATION = 5000;

    // Comprueba si se ha producido un error anteriormente

    setTimeout(() => {

      // Lanzamos la petición
      fetch("error.json", { method: "GET" }).then((data) => {
        data.json().then((json) => {

          // Comprobamos si hay error

          document.querySelector("#error").innerText = json.error;
          let message = document.querySelector("#centerMessage");
          message.classList.remove("hidden");

          // Mostramos la notificación por un tiempo

          setTimeout(() => {
            document.querySelector("#messageNotificationError").classList.add("notShowNotification");
            setTimeout(() => {
              message.classList.add("hidden");
            }, 1000)
          }, TIME_NOTIFICATION);
        })});
    }, DELAY_NOTIFICATION);
    

// Cambia el tema con animación
theme_button.addEventListener("click", () => {
  theme_image.classList.add("translate");

  setTimeout(() => {
    // Si está habilitaado lo deshabilitamos
    if (dark_mode) {
      dark_mode = false;
      body.classList.remove("dark");
      theme_image.setAttribute("src", ROOT_IMG + DARK_ICON + ".png");
      browser.setAttribute("content", WHITE);
    } else {
       // Si está desabilitamos lo habiliamos
      dark_mode = true;
      body.classList.add("dark");
      theme_image.setAttribute("src", ROOT_IMG + LIGHT_ICON + ".png");
      browser.setAttribute("content", DARK);
    }
    theme_image.classList.remove("translate");
  }, 1000);
});

let darkMode = false;                 // Habilita el modo oscuro

// Oculta la pantalla de bienvenida
setTimeout(() => {
  intro.classList.add("hidden");
  browser.setAttribute("content", WHITE);
}, 6500);


// Cambia el texto del inicio

let intervalText = setInterval(() => {
  let es = document.querySelector("#idioma-text-es");
  es.classList.toggle("hideDown");
  es.classList.toggle("showUp");

  let en = document.querySelector("#idioma-text-en");
  en.classList.toggle("showUp");
  en.classList.toggle("hideDown");
}, 3000);

// Muestra la pestaña de temperatura

temperatureSectionButton.addEventListener("click", () => {
  temperatureSectionButton.classList.add("optionSelected");
  motionSection.classList.remove("optionSelected");
  sensorTHpage.classList.remove("hidden");
  sensorPIRpage.classList.add("hidden");
});

// Muestra la pestaña de movimientos
motionSection.addEventListener("click", () => {
  motionSection.classList.add("optionSelected");
  temperatureSectionButton.classList.remove("optionSelected");
  sensorTHpage.classList.add("hidden");
  sensorPIRpage.classList.remove("hidden");
});

// Redirige hasta el proyecto en github

document.querySelector("#github").addEventListener("click", () => {
  window.location.href = PROYECT_URL;
});

// Cierra la primera ventana
document.querySelector("#configure").addEventListener("click", () => {
  display1.classList.add("notShowing");

  setTimeout(() => {
    secondDisplay();
  }, 500);
});


// Muestra el modal de prueba de sensores
testButton.addEventListener("click", () => {
  updateSensors();
  testSensorsModal.classList.remove("hidden");
});

// Cierra lso websockets
function closeWebsocket() {
  websocket.close();
  websocket2.close();
}

// Intenta reestablecer la conexión
tryButton.addEventListener("click", () => {
  updateSensors();
});


// Escucha el formulario del usuario y decide si habilitar el botón o no de guardar

document.querySelector("#formUser").addEventListener("keyup", () => {
  
  let buttom = document.querySelector("#sendData");
  
  if (document.querySelector("#userInput").value.length == 0 || document.querySelector("#passwordInput").value.length == 0) {
    buttom.setAttribute("disabled", "");
  }
  else {
    buttom.removeAttribute("disabled");
  }
});


// Oculta o muestra la contraseña
document.querySelector("#showHiddenPasswordUser").addEventListener("click", (event) => {
  let inputPassword = document.querySelector("#passwordInput");

  if (inputPassword.type == "password") {
    inputPassword.setAttribute("type", "text");
    event.target.src = "file/img/passwordView.png";
  }
  else {
    event.target.src = "file/img/passwordHidden.png";
    inputPassword.setAttribute("type", "password");
  }
})




// Cierra el modal de prueba de sensores
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


// Muestra la segunda pantalla
function secondDisplay() {
  display1.classList.add("hidden");
  display2.classList.remove("hidden");
}

document.querySelector("#refresh").addEventListener("click", refreshNetworks);


// Refresca las redes
function refreshNetworks() {

  // Mostramos la carga

  listOfNetworks.innerHTML =
    '<div class="loading" id="loadingNetworks"><img src="file/img/loading.gif"></div>';

    // Obtenemos las redes

  fetch("network", { method: "GET" }).then((data) => {
    data.json().then((json) => {
      
      // Las ordenamos

      let networksSorted = json.networks.sort(compareSignal);

      listOfNetworks.innerHTML = "";
      
      // Añadimos cada red con su señal correspondiente

      networksSorted.forEach((network) => {
        let li = document.createElement("li");
        let img = document.createElement("img");

        if (network.signal > 70) {
          img.src = "file/img/wifi100.png";
        } else if (network.signal > 40) {
          img.src = "file/img/wifi80.png";
        } else {
          img.src = "file/img/wifi0.png";
        }

        li.appendChild(img);

        let ssidName = document.createElement("p");
        ssidName.innerText = network.ssid;

        li.appendChild(ssidName);

        // Si está encriptada le añadimos candado

        if (network.encryptionType != "open") {
          let lock = document.createElement("img");

          lock.classList.add("lock");
          lock.src = "file/img/lock.png";

          li.appendChild(lock);
        }

        listOfNetworks.appendChild(li);
      });
    });
  });
}


// Compara la señal de las redes wifi
function compareSignal(a, b) {
  if (parseInt(b.signal) < parseInt(a.signal)) {
    return -1;
  }
  if (parseInt(b.signal) > parseInt(a.signal)) {
    return 1;
  }
  return 0;
}

// Muestra la 3a ventana

function thirdDisplay() {
  display2.classList.add("notShowing");

  refreshNetworks();

  setTimeout(() => {
    display2.classList.add("hidden");
    display3.classList.remove("hidden");
  }, 500);
}


// Muestra la 4a pantalla

function fourDisplay() {
  display3.classList.add("notShowing");

  setTimeout(() => {
    display3.classList.add("hidden");
    display4.classList.remove("hidden");
  }, 500);
}


// Actualiza los sensores

function updateSensors() {
  runWebSocket();
}

// Muestra la pantalla de carga

function showLoading() {
  loadingPage.classList.remove("hidden");
  errorSensorPage.classList.add("hidden");
  sensorsGraphics.forEach((graphic) => {
    graphic.classList.add("hidden");
  });
}

// Muestra la pestaña de sensores de ambiente

function showSensorsGraphics() {
  sensorsGraphics.forEach((graphic) => {
    graphic.classList.remove("hidden");
  });
  loadingPage.classList.add("hidden");
  errorSensorPage.classList.add("hidden");
}

// Muestra el error producido

function showError(error) {
  sensorsGraphics.forEach((graphic) => {
    graphic.classList.add("hidden");
  });
  loadingPage.classList.add("hidden");
  errorSensorPage.classList.remove("hidden");
  document.querySelector("#showErrorSensor").innerText = error;
}

// Inicializa el websocket
function runWebSocket() {
  
  // Muestra la pantalla de carga
  showLoading();

  // Iniciamos los websockets de los sensores

  websocket = new WebSocket("ws://" + location.hostname + ":81/", ["arduino"]);

  websocket2 = new WebSocket("ws://" + location.hostname + ":82/", ["arduino"]);

  // AL abrir mostramos los datos

  websocket.onopen = () => {
    showSensorsGraphics();
  };

  // EN caso de error mostramos el mismo

  websocket.onerror = () => {
    showError("Servidor no disponible");
  };

  // En caso de mensaje indicamos los nuevos valores

  websocket.onmessage = (data) => {
    refreshSensorTH(data.data);
  };

  // En caso de mensaje mostramos el movimiento

  websocket2.onmessage = () => {
    showMotion();
  };
}

// Detecta un movimiento y lo muestra por pantalla
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

// Obtiene el valor del sensor de temperatura

function refreshSensorTH(response) {
  let responseJson = JSON.parse(response);

  if (responseJson.error != undefined) {
    // EN caso de error mostramos el error

    showError(responseJson.error);
  } else {

    // Si no mostramos los datos

    showSensorsGraphics();
    temperatureGraphic.style =
      "--angule: " +
      parseInt(responseJson.celcius) * 3.6 +
      "deg; --text: '" +
      responseJson.celcius +
      " C° " +
      responseJson.fahrenheit +
      " F'";
    humidityGraphic.style =
      "--angule: " +
      parseInt(responseJson.humidity) * 1.8 +
      "deg; --text: '" +
      responseJson.humidity +
      " %'";
  }
}

// Muestra la segunda pantalla
document.querySelector("#next2").addEventListener("click", () => {
  thirdDisplay();
});


// Muestra el modal al pulsar en una red wifi

listOfNetworks.addEventListener("click", (event) => {
  let target = event.target;

  // Si no es una red wifi retornamos 

  if (target.matches("li > *")) {
    target = target.parentNode;
  } else if (!target.matches("li")) {
    return;
  }

  // Obtenemos el ssid

  ssidSelected = target.querySelector("p").innerText;

  // Mostramos el modal con los datos de la red

  if (target.querySelectorAll("img").length != 1) {
    wifiPasswordModalBack.classList.remove("hidden");
    wifiPasswordModal.querySelector("#titleWifiWithPassword").innerText =
      ssidSelected;
  } else {
    password = "";
    fourDisplay();
  }
});


// Cierra el modal del wifi
closeWifiModal.addEventListener("click", () => {
  wifiPasswordModal.classList.add("hiddenAnimation");
  wifiPasswordModalBack.classList.add("hiddenBack");

  setTimeout(() => {
    wifiPasswordModalBack.classList.remove("hiddenBack");
    wifiPasswordModalBack.classList.add("hidden");
    wifiPasswordModal.classList.remove("hiddenAnimation");
  }, 300);
});

// Muestra u oculta la contraseña
document
  .querySelector("#showHiddenPassword")
  .addEventListener("click", (event) => {
    if (passwordWifiInput.type == "password") {
      passwordWifiInput.type = "text";
      event.target.src = "file/img/passwordView.png";
    } else {
      passwordWifiInput.type = "password";
      event.target.src = "file/img/passwordHidden.png";
    }
  });


// Comprueba la longitud de la contraseña 
passwordWifiInput.addEventListener("keyup", (event) => {
  if (event.target.value.length > 7) {
    submitButton.removeAttribute("disabled");
  } else {
    submitButton.setAttribute("disabled", "");
  }
});


// Muestra la 4a pantalla
submitButton.addEventListener("click", (event) => {
  event.preventDefault();
  password = passwordWifiInput.value;
  closeWifiModal.click();
  fourDisplay();
});




// Cierra el modal de usuario con animación
document.querySelector("#closeUserModal").addEventListener("click", () => {
  let modal = document.querySelector("#userAndPasswordModal");
  let modalBack = document.querySelector("#userPasswordModalBack"); 

  modal.classList.add("hiddenAnimation");
  modalBack.classList.add("hiddenBack");

  setTimeout(() => {
    modalBack.classList.remove("hiddenBack");
    modal.classList.add("hidden");
    modalBack.classList.add("hidden");
    modal.classList.remove("hiddenAnimation");
  }, 300);
});


// Oculta el modal al darle al boton de finalizar
document.querySelector("#finishButton").addEventListener("click", (event) => {
  document.querySelector("#userPasswordModalBack").classList.remove("hidden");
  document.querySelector("#userAndPasswordModal").classList.remove("hidden");
});

// Al hacer click al enviar envía la petición
document.querySelector("#sendData").addEventListener("click", (event) => {
  event.preventDefault();

  // Deshabilitamos el botón

  event.target.setAttribute("disabled", "");

  // Agregamos los datos

  user = document.querySelector("#userInput").value;
  passwordUser = document.querySelector("#passwordInput").value;
  

  // Enviamos los datos

  sendData();
});

// Envía los datos y configura el dispositivo

function sendData() {
  
  // Lanzamos la petición con todos los datos

  fetch("configure", {
    method: "POST",
    headers: {
      "Accept": "application/json",
      "Content-Type": "application/json",
    },
    body: JSON.stringify({
      ssid: ssidSelected,
      password: password,
      user: user,
      passwordUser: passwordUser
    }),
  }).then(() => {
      // Si todo ha salido correcto mostramos la pantalla de reseteo
      document.querySelector("#reboot-display").classList.remove("hidden");
  });


}