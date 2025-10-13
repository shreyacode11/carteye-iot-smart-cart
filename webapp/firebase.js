import { initializeApp } from "firebase/app";
import { getDatabase } from "firebase/database"; // for Realtime Database (optional)
import { getAuth } from "firebase/auth"; // for authentication (optional)

const firebaseConfig = {
  apiKey: import.meta.env.VITE_FIREBASE_API_KEY,
  authDomain: import.meta.env.VITE_FIREBASE_AUTH_DOMAIN,
  projectId: import.meta.env.VITE_FIREBASE_PROJECT_ID,
};

// Initialize Firebase
const app = initializeApp(firebaseConfig);

// Optional: initialize Firebase services
export const db = getDatabase(app);
export const auth = getAuth(app);
export default app;
