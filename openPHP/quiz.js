const quizContainer = document.querySelector(".quiz-container");
const question = document.querySelector(".quiz-container .question");
const options = document.querySelector(".quiz-container .options");
const nextBtn = document.querySelector(".quiz-container .next-btn");
const quizResult = document.querySelector(".quiz-result");
const startBtnContainer = document.querySelector(".start-btn-container");
const startBtn = document.querySelector(".start-btn-container .start-btn");

let questionNumber = 0;
let score = 0;
const MAX_QUESTIONS = 14;
let timerInterval;
let quizData; // Variable to store fetched quiz data

// Function to shuffle array elements
const shuffleArray = (array) => {
  if (Array.isArray(array) && array.length > 0) {
    return array.slice().sort(() => Math.random() - 0.5);
  } else {
    console.error('Invalid array provided to shuffleArray function');
    return [];
  }
};

// Function to check user's answer
const checkAnswer = (e) => {
  let userAnswer = e.target.textContent;
  let correctAnswer = quizData[questionNumber].options[quizData[questionNumber].correct];

  // Store the user's answer in local storage
  localStorage.setItem(`userAnswer_${questionNumber}`, userAnswer);

  // Highlight correct answer
  let allOptions = document.querySelectorAll(".quiz-container .option");
  allOptions.forEach((option) => {
    if (option.textContent === correctAnswer) {
      option.classList.add("correct");
    }
  });

  // Check if the user's answer is correct
  if (userAnswer === correctAnswer) {
    score++;
    e.target.classList.add("correct");
  } else {
    e.target.classList.add("incorrect");
  }

  // Disable all options after answering
  allOptions.forEach((o) => {
    o.classList.add("disabled");
  });
};




// Function to create question and options
const createQuestion = () => {
  clearInterval(timerInterval);

  let secondsLeft = 10; // Reset timer to 10 seconds for each question
  const timerDisplay = document.querySelector(".quiz-container .timer");
  timerDisplay.classList.remove("danger");

  timerDisplay.textContent = `Time Left: ${secondsLeft.toString().padStart(2, "0")} seconds`;

  timerInterval = setInterval(() => {
    timerDisplay.textContent = `Time Left: ${secondsLeft.toString().padStart(2, "0")} seconds`;
    secondsLeft--;

    if (secondsLeft < 3) {
      timerDisplay.classList.add("danger");
    }

    if (secondsLeft < 0) {
      clearInterval(timerInterval);
      displayNextQuestion();
    }
  }, 1000);

  options.innerHTML = ""; // Clear previous options

  // Display question text
  question.innerHTML = `<span class='question-number'>${questionNumber + 1}/${MAX_QUESTIONS}</span>${quizData[questionNumber].question}`;

  // Check if options exist and is an array
  const questionOptions = quizData[questionNumber].options;
  if (Array.isArray(questionOptions) && questionOptions.length > 0) {
    // Shuffle the options
    let shuffledOptions = shuffleArray(questionOptions);

    // Loop over options and create buttons
    shuffledOptions.forEach((o) => {
      const option = document.createElement("button");
      option.classList.add("option");
      option.innerHTML = o;
      option.addEventListener("click", (e) => {
        checkAnswer(e);
      });
      options.appendChild(option);
    });
  } else {
    console.error('Options for the current question are missing or invalid.');
  }
};

// Function to display quiz result
const displayQuizResult = () => {
  quizResult.style.display = "flex";
  quizContainer.style.display = "none";
  quizResult.innerHTML = "";

  const resultHeading = document.createElement("h2");
  resultHeading.innerHTML = `You have scored ${score} out of ${MAX_QUESTIONS}`;
  quizResult.appendChild(resultHeading);

  for (let i = 0; i < MAX_QUESTIONS; i++) {
    const resultItem = document.createElement("div");
    resultItem.classList.add("question-container");

    const userAnswer = localStorage.getItem(`userAnswer_${i}`);
    const correctAnswer = quizData[i].options[quizData[i].correct];

    resultItem.innerHTML = `
      <div class="question">Question ${i + 1}: ${quizData[i].question}</div>
      <div class="user-answer">Your answer: ${userAnswer}</div>
      <div class="correct-answer">Correct answer: ${correctAnswer}</div>
    `;

    if (userAnswer === correctAnswer) {
      resultItem.classList.add("correct");
    } else {
      resultItem.classList.add("incorrect");
    }

    quizResult.appendChild(resultItem);
  }

  const retakeBtn = document.createElement("button");
  retakeBtn.classList.add("retake-btn");
  retakeBtn.innerHTML = "Retake Quiz";
  retakeBtn.addEventListener("click", retakeQuiz);
  quizResult.appendChild(retakeBtn);
};

// Function to display next question
const displayNextQuestion = () => {
  if (questionNumber >= MAX_QUESTIONS - 1) {
    displayQuizResult();
    return;
  }

  questionNumber++;
  createQuestion();
};

// Function to reset quiz and retake
const retakeQuiz = () => {
  questionNumber = 0;
  score = 0;
  resetLocalStorage();
  createQuestion();
  quizResult.style.display = "none";
  quizContainer.style.display = "block";
};

// Event listener for next button
nextBtn.addEventListener("click", displayNextQuestion);

// Event listener for start button
startBtn.addEventListener("click", () => {
  startBtnContainer.style.display = "none";
  quizContainer.style.display = "block";
  fetchQuizData(); // Fetch quiz data when start button is clicked
});

// Function to fetch quiz data from server
const fetchQuizData = () => {
  console.log("Fetching quiz data...");
  fetch("quiz_data.php") // Replace "quiz_data.php" with your actual PHP file path
    .then(response => {
      if (!response.ok) {
        throw new Error('Network response was not ok');
      }
      return response.json();
    })
    .then(data => {
      console.log("Fetched quiz data:", data);
      if (!Array.isArray(data) || data.length === 0) {
        throw new Error('Quiz data is empty or not in the correct format');
      }
      quizData = data;
      console.log("Quiz data stored:", quizData);
      createQuestion();
    })
    .catch(error => console.error("Error fetching quiz data:", error));
};

// Function to reset local storage
const resetLocalStorage = () => {
  for (let i = 0; i < MAX_QUESTIONS; i++) {
    localStorage.removeItem(`userAnswer_${i}`);
  }
};