import { initializeApp } from "https://www.gstatic.com/firebasejs/9.6.10/firebase-app.js";
import { getAnalytics } from "https://www.gstatic.com/firebasejs/9.6.10/firebase-analytics.js";
import { getAuth, signInWithEmailAndPassword, createUserWithEmailAndPassword } from "https://www.gstatic.com/firebasejs/9.6.10/firebase-auth.js";

const firebaseConfig = {
    apiKey: "AIzaSyC7RN3o4MjeqL32ewvDq1cPIRW7ohKkCW4",
    authDomain: "handberton-96e2c.firebaseapp.com",
    databaseURL: "https://handberton-96e2c-default-rtdb.europe-west1.firebasedatabase.app",
    projectId: "handberton-96e2c",
    storageBucket: "handberton-96e2c.appspot.com",
    messagingSenderId: "231357236145",
    appId: "1:231357236145:web:a23cc4334d1bc95bfbf390",
    measurementId: "G-LHJE05Y2RG"
  };

const app = initializeApp(firebaseConfig);
const analytics = getAnalytics(app);
const auth = getAuth(app);

const submitButton = document.getElementById("submit");
const signupButton = document.getElementById("sign-up");
const emailInput = document.getElementById("email");
const passwordInput = document.getElementById("password");
const main = document.getElementById("main");
const createacct = document.getElementById("create-acct")

const signupEmailIn = document.getElementById("email-signup");
const confirmSignupEmailIn = document.getElementById("confirm-email-signup");
const signupPasswordIn = document.getElementById("password-signup");
const confirmSignUpPasswordIn = document.getElementById("confirm-password-signup");
const createacctbtn = document.getElementById("create-acct-btn");

const returnBtn = document.getElementById("return-btn");

var email, password, signupEmail, signupPassword, confirmSignupEmail, confirmSignUpPassword;

createacctbtn.addEventListener("click", function() {
  var isVerified = true;

  signupEmail = signupEmailIn.value;
  confirmSignupEmail = confirmSignupEmailIn.value;
  if(signupEmail != confirmSignupEmail) {
      window.alert("Email fields do not match. Try again.")
      isVerified = false;
  }

  signupPassword = signupPasswordIn.value;
  confirmSignUpPassword = confirmSignUpPasswordIn.value;
  if(signupPassword != confirmSignUpPassword) {
      window.alert("Password fields do not match. Try again.")
      isVerified = false;
  }
  
  if(signupEmail == null || confirmSignupEmail == null || signupPassword == null || confirmSignUpPassword == null) {
    window.alert("Please fill out all required fields.");
    isVerified = false;
  }
  
  if(isVerified) {
    createUserWithEmailAndPassword(auth, signupEmail, signupPassword)
      .then((userCredential) => {
      // Signed in 
      const user = userCredential.user;
      // ...
      window.alert("Success! Account created.");
      window.location.href = 'AI-pour-Handberton/handberton-website-proto.html'; // Redirect to the specified page
    })
    .catch((error) => {
      const errorCode = error.code;
      const errorMessage = error.message;
      // ..
      window.alert("Error occurred. Try again.");
    });
  }
});

submitButton.addEventListener("click", function() {
  email = emailInput.value;
  console.log(email);
  password = passwordInput.value;
  console.log(password);

  signInWithEmailAndPassword(auth, email, password)
    .then((userCredential) => {
      // Signed in
      const user = userCredential.user;
      console.log("Success! Welcome back!");
      window.alert("Success! Welcome back!");
      window.location.href = 'AI-pour-Handberton/handberton-website-proto.html'; // Redirect to the specified page
      // ...
    })
    .catch((error) => {
      const errorCode = error.code;
      const errorMessage = error.message;
      console.log("Error occurred. Try again.");
      window.alert("Error occurred. Try again.");
    });
});


document.getElementById('sign-up').addEventListener('click', () => {
  document.getElementById('main').classList.remove('active');
  setTimeout(() => {
      document.getElementById('main').style.display = 'none';
      document.getElementById('create-acct').style.display = 'block';
      setTimeout(() => {
          document.getElementById('create-acct').classList.add('active');
      }, 10);
  }, 300);
});

document.getElementById('return-btn').addEventListener('click', () => {
  document.getElementById('create-acct').classList.remove('active');
  setTimeout(() => {
      document.getElementById('create-acct').style.display = 'none';
      document.getElementById('main').style.display = 'block';
      setTimeout(() => {
          document.getElementById('main').classList.add('active');
      }, 10);
  }, 300);
});

// Initial activation
window.addEventListener('DOMContentLoaded', () => {
  document.getElementById('main').classList.add('active');
});
