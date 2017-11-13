<?php

	include("connect.php"); 	
	
	$link=Connection();
	$response=array();
	$result=mysql_query("SELECT * FROM `temphumi` ORDER BY `date` DESC",$link);
		
					while($row = mysql_fetch_array($result)){
						
						array_push($response,array("date"=>$row[0],"Temp"=>$row[1],"TempF"=>$row[2],"Humi"=>$row[3]));
					}
					
					
	echo json_encode(array("temphumi"=>`$response));
	mysql_close($link)
	?>
	
	
	
	
