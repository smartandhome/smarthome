<?php

	include("connect.php"); 	
	
	$link=Connection();

	$output=mysql_query("SELECT * FROM `check_in` ORDER BY `date` DESC",$link);
/*?>

<html>
   <head>
      <title>User checkin</title>
   </head>
<body>
   <h1>Checkin time with ID values</h1>

   <table border="1" cellspacing="5" cellpadding="2">
		<tr>
			<td>&nbsp;User_ID &nbsp;</td>
			<td>&nbsp;Date &nbsp;</td>
			<td>&nbsp;UID &nbsp;</td>
		</tr>

      <?php 
		  if($result!==FALSE){
		     while($row = mysql_fetch_array($output)) {
		        printf("<tr><td> &nbsp; %s </td><td> &nbsp; %s </td><td> &nbsp;  %u &nbsp; </td></tr>", 
		           $row[User_ID],$row[Date],$row["UID"]);
		     }
		     mysql_free_result($output);
		     mysql_close();
		  }*/
		  if(mysql_num_rows($output)>0){
			  $response["Checkin"]=array();
			  while($row=mysql_fetch_array($output)){
				  $Entry_auth= array();
				  $Entry_auth["User_ID"] = $row["User_ID"];
				  $Entry_auth["Date"] = $row["Date"];
				  $Entry_auth["UID"] = $row["UID"];
				  array_push($response["Checkin"],$Entry_auth);
							
			  }
			  $response["success"]=1;
			  echo json_encode($response);
		  } else{ 
			  $response["success"] = 0;
			  $response["message"] = "details not available";
			  
		  }
     header('Content-type: application/json');
	 echo json_encode($response);

/*   </table>
</body>
</html> */ ?>