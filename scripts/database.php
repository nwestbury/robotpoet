<?php

$user = '[redacted]';
$password = '[redacted]';
$mysqli = new mysqli("localhost", $user, $password, "poetsthesaurus");
if(mysqli_connect_error())  die('Connect Error (' . mysqli_connect_errno() . ') ' . mysqli_connect_error());

?>