<!DOCTYPE html>
<html>

<head>
	<meta charset="UTF-8">
	<title>Robot Poet: About</title>
	<meta name="application-name" content="Robot Poet">
	<meta name="author" content="Nathan Liebrecht, Nicholas Westbury">
	<meta name="description" content="Save time, find related rhythms!">
	<meta name="keywords" content="Poet's, Poets, Thesaurus, Rhythm, Relatedness, Word, Lexical">

	<link rel="shortcut icon" href="/favicon.ico" type="image/x-icon">
	<link rel="icon" href="/favicon.ico" type="image/x-icon">
	<link href='http://fonts.googleapis.com/css?family=Lora:400,400italic' rel='stylesheet' type='text/css'>
	<link href="css/jquery.switchButton.css" rel="stylesheet" type='text/css'>
	<link href="css/style.css" rel='stylesheet' type='text/css'>
	<script src ="scripts/ga.js"></script>
</head>


<body>
	<div id="wrapper">
		<?php include_once("templates/title.php") ?>
		<p id="about">The Robot Poet is a rhyming experiment. The goal of the project is to find related rhyming words. The relationships where created (with permission) using <a href="http://www.psych.ualberta.ca/~westburylab/">HiDEx</a> and the precomputed 1.64 billion word-relationships, read more about it there.
		 Of course, due to the complexity of the problem, there will be mistakes. If you have any suggestions or fan poetry feel free to send them to: <a href="mailto:poetsthesaurus@gmail.com">poetsthesaurus@gmail.com</a> -- we appreciate it! We hope you enjoy your visit!   		
		</p>
		<br><br>
		<?php include_once("templates/footer.php") ?>
	</div>
</body>

</html>
