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
let next = document.querySelector("#next");
let testButton = document.querySelector("#test");
let testModal = document.querySelector("#testModal");
let testSensorsModal = document.querySelector("#testSensorsModal");
let closeModalTest = document.querySelector("#closeModalTest");
let temperatureSectionButton = document.querySelector("#temperatureSection");
let motionSection = document.querySelector("#motionSection");


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
    } 
    else {
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
});

motionSection.addEventListener("click", () => {
  motionSection.classList.add("optionSelected");
  temperatureSectionButton.classList.remove("optionSelected");

});


document.querySelector("#es").addEventListener("click", () => {
  setLanguage("es");
});

document.querySelector("#en").addEventListener("click", () => {
  setLanguage("en");
});

testButton.addEventListener("click", () => {
  testSensorsModal.classList.remove("hidden");
});

closeModalTest.addEventListener("click", () => {
  testModal.classList.add("hiddenAnimation");
  testSensorsModal.classList.add("hiddenBack");

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
  display1.classList.add("hidden")
  display2.classList.remove("hidden");
  next.classList.remove("hidden");
}


