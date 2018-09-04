<?php
require_once("database.php");

function sendVoteToTable($connect){
	/*$_POST['vote'] 0 = downvote 1= upvote $_POST['word1'] must always be b4 $_POST['word2'] alphabetically (A-Z)*/
	if(isset($_POST['vote']) and isset($_POST['word1']) and $_POST['word1'] != "" and isset($_POST['word2']) and $_POST['word1'] != ""){
		$guessIP = $connect->real_escape_string($_SERVER['SERVER_ADDR']);
		$reply = $connect->query("SELECT votes FROM ipvotes WHERE ip = '{$guessIP}';");
		$arrayOfVotes = $reply->fetch_array(MYSQLI_NUM);
		
		if(isset($arrayOfVotes[0])){
			if($arrayOfVotes[0] < 5)
				$connect->query("UPDATE ipvotes SET votes = votes + 1 WHERE ip='{$guessIP}'");
			else
				return "Only 5 votes per day";
		}else{
			$connect->query("INSERT INTO ipvotes (ip, votes) VALUES ('{$guessIP}', 1)");
		}
		
		$upordownvote = $_POST['vote']=="1" ? "upvotes" : "downvotes";
		$words = array($connect->real_escape_string($_POST['word1']), $connect->real_escape_string($_POST['word2']));
		sort($words, SORT_FLAG_CASE | SORT_STRING); //sort words alphabetically
		$connect->query("INSERT INTO releatedvotes (word1, word2, {$upordownvote})
		VALUES ('{$words[0]}', '{$words[1]}', 1)
		ON DUPLICATE KEY UPDATE {$upordownvote} = {$upordownvote} + 1");
		return "Thanks!";
	}else{
		return "Failed. :(";
	}
}

echo sendVoteToTable($mysqli);
?>