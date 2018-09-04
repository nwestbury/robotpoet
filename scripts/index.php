<!DOCTYPE html>
<html>

<head>
	<meta charset="UTF-8">
	<title>The Poet's Thesaurus</title>
	<meta name="application-name" content="The Poet's Thesaurus">
	<meta name="author" content="Nathan Liebrecht, Nicholas Westbury">
	<meta name="description" content="Save time, find related rhythms!">
	<meta name="keywords" content="Poet's, Poets, Thesaurus, Rhythm, Relatedness, Word, Lexical, Words, Word, Dictonnary">
</head>


<body>
	<div id="wrapper">
		<form id="search_form" method="POST" action="getDataNew.php">
			Find words that rhyme with <input type="text" name="word" id="wordInput"> and are related to <input type="text" name="lexhint" id="lexhintInput">
			<input type="submit" value="Search">
		</form>
	</div>
</body>

</html>