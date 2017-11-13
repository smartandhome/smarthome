<?php

	include("connect.php"); 	
	
	$link=Connection();
	$response=array();
	$result=mysql_query("SELECT * FROM `temphumi` ORDER BY `date` DESC",$link);
/*?>

<html>
   <head>
      <title>Sensor Data</title>
   </head>
<body>
   <h1>Temperature / moisture sensor readings</h1>

   <table border="1" cellspacing="1" cellpadding="1">
		<tr>
			
			<td>&nbsp;date&nbsp;</td>
			<td>&nbsp;Temp &nbsp;</td>
			<td>&nbsp;TempF &nbsp;</td>
			<td>&nbsp;Humi &nbsp;</td>
		</tr>

      <?php */
		 /* if($result!==FALSE){
		     while($row = mysql_fetch_array($result)) {
		        printf("<tr><td> &nbsp; %s </td><td> &nbsp; %s </td><td> &nbsp; %s </td><td> &nbsp; %s &nbsp; </td></tr>", 
		           $row[date],$row["Temp"],$row["TempF"], $row["Humi"]);
		     }
		     mysql_free_result($result);
		     mysql_close();
		  }*/
				if(mysql_num_rows($result)>0){
					$response["temphumi"]= array();
					while($row = mysql_fetch_array($result)){
						$temp_humi = array();
						$temp_humi["date"] = $row["date"];
						$temp_humi["Temp"] = $row["Temp"];
						$temp_humi["TempF"] = $row["TempF"];
						$temp_humi["Humi"] = $row["Humi"];
						//pushing details into final array
						array_push($response["temphumi"],$temp_humi);
					}
					
					//success
					$response["success"]=1;
					//echoing json response
					echo json_encode($response);
				} else{
					$response["success"] =  0;
					$response["message"] = "details not available";
					}
			
	header('Content-type: application/json');			
	echo json_encode($response);
			
/*</body>
 </table>
</html> */
			?>
