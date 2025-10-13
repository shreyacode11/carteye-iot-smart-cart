//Import the functions you need from the SDKs you need
import { initializeApp } from "firebase/app";
import { getAnalytics } from "firebase/analytics";
// TODO: Add SDKs for Firebase products that you want to use
// https://firebase.google.com/docs/web/setup#available-libraries

// Your web app's Firebase configuration
// For Firebase JS SDK v7.20.0 and later, measurementId is optional
const firebaseConfig = {
  apiKey: "AIzaSyBqdaccX7faYH6vXnDmYcbPfRyoYHJYUcA",
  authDomain: "smartcart6.firebaseapp.com",
  databaseURL: "https://smartcart6-default-rtdb.firebaseio.com",
  projectId: "smartcart6",
  storageBucket: "smartcart6.firebasestorage.app",
  messagingSenderId: "317170991723",
  appId: "1:317170991723:web:e9e133adcb539da341272e",
  measurementId: "G-DTEK53LQYZ"
};

// Initialize Firebase
const app = initializeApp(firebaseConfig);
const analytics = getAnalytics(app);