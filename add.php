<?php
	include("connect.php");
  	
  	$link=Connection();
	
	
	$Temperature=$_GET["Temp"];
	$TemperatureF=$_GET["TempF"];
	$Humidity=$_GET["Humi"];

	$query = "INSERT INTO `temphumi` (`Temp`,`TempF`, `Humi`) 
		VALUES ('".$Temperature."','".$TemperatureF."','".$Humidity."')"; 
   	
   	mysql_query($query,$link);
	mysql_close($link);

   	header("Location: index1.php");
?>

