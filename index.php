<!DOCTYPE html>
<html>

<head>
	<meta charset="UTF-8">
	<title>Robot Poet</title>
	<meta name="application-name" content="Robot Poet">
	<meta name="author" content="Nathan Liebrecht, Nicholas Westbury">
	<meta name="description" content="Save time, find related rhythms!">
	<meta name="keywords" content="Poet's, Poets, Thesaurus, Rhythm, Relatedness, Word, Lexical, Words, Word, Dictonnary">
	
	<!-- <link href='http://fonts.googleapis.com/css?family=Lora:400,400italic' rel='stylesheet' type='text/css'> !-->
	<link href='https://fonts.googleapis.com/css?family=Roboto' rel='stylesheet' type='text/css'>
	<link href="css/jquery.switchButton.css" rel="stylesheet" type='text/css'>
	<link href="//code.jquery.com/ui/1.10.4/themes/smoothness/jquery-ui.css" rel="stylesheet" type='text/css'>
	<link href="css/style.css" rel='stylesheet' type='text/css'>
	<link rel="shortcut icon" href="/favicon.ico" type="image/x-icon">
	<link rel="icon" href="/favicon.ico" type="image/x-icon">
	<script src="http://ajax.googleapis.com/ajax/libs/jquery/2.1.0/jquery.min.js"></script>
    <script src="http://ajax.googleapis.com/ajax/libs/jqueryui/1.10.4/jquery-ui.min.js"></script>
	
	<script src ="scripts/jquery.switchButton.js"></script>
	<script src ="scripts/sortScript.js"></script>
	<script src ="scripts/ga.js"></script>
</head>


<body>
	<div id="wrapper">
		<?php include_once("templates/title.php") ?>
		<form id="search_form" method="POST">
			<input type="text" name="word" value="Word that rhymes with..." id="wordInput" maxlength="33">
			<input type="text" name="lexhint" value="And related to..." id="lexhintInput" maxlength="33">
			<input type="submit" value="Search">
			<br><br><div class="centered"><input id="toggleButton" type="checkbox" checked></div>
		</form>
		<div id="loading"><img src="images/loader.gif"/></div>
		<pre id="errorwrapper"></pre>
		<div id="resultwrapper"></div>
		<?php include_once("templates/footer.php") ?>
	</div>
</body>

</html>
