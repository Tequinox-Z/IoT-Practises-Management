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


document.querySelector("#es").addEventListener("click", () => {
    setLanguage("es");
});

document.querySelector("#en").addEventListener("click", () => {
    setLanguage("en");
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
}