<?php
// Database connection parameters
$servername = "localhost";
$username = "polyprep";
$password = "poly@321";
$dbname = "quizdat";

try {
    // Create a new PDO instance
    $conn = new PDO("mysql:host=$servername;dbname=$dbname", $username, $password);
    
    // Set PDO error mode to exception
    $conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
    
    // SQL query to fetch quiz data
    $sql = "SELECT question, opt_A, opt_B, opt_C, opt_D, correct FROM dbms"; // Replace "dbms" with your actual table name
    
    // Prepare and execute the query
    $stmt = $conn->query($sql);
    
    // Fetch quiz data as associative array
    $quizData = $stmt->fetchAll(PDO::FETCH_ASSOC);
    
    // Check if quiz data is not empty
    if ($quizData) {
        // Modify the structure to include the options array with correct answer for each question
        foreach ($quizData as &$question) {
            $options = [$question['opt_A'], $question['opt_B'], $question['opt_C'], $question['opt_D']];
            shuffle($options); // Shuffle the options
            $question['options'] = $options;
            // Include the correct answer within the options array
            $question['correct'] = array_search($question['correct'], $options);
            // Unset the individual option fields to avoid redundancy
            unset($question['opt_A'], $question['opt_B'], $question['opt_C'], $question['opt_D']);
        }
        // Output quiz data as JSON
        echo json_encode($quizData);
    } else {
        echo json_encode(array()); // Return an empty array if no data found
    }
} catch(PDOException $e) {
    // Handle database connection errors
    echo "Connection failed: " . $e->getMessage();
}
?>
