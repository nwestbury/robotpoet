<?php
chdir(__DIR__);

function isValidWord($string){
	return isset($string) && $string && strlen($string)<40;
}

$word = strtolower($_POST['word']);
$lexicalHint = strtolower($_POST['lexhint']);

if(isValidWord($word)){
	$path = getcwd();
	chdir($path); 
	
	$command = "";
	if(isValidWord($lexicalHint))
		$command = "./backend {$word} {$lexicalHint}";
	else
		$command = "./backend {$word}";
	
	$a = exec($command, $jsonOutput, $successInt);
    
	if($successInt == 0){
		echo $jsonOutput[0];
	}else{
		die("Failed to retrieve results for $word, sorry :(");
	}
    

}else{
	die("No word to compute");
}
?>
