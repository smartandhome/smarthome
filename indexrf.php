<?php

	include("connect.php"); 	
	
	$link=Connection();

	$output=mysql_query("SELECT * FROM `check_in` ",$link);
?>

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
		  }
      ?>

   </table>
</body>
</html>
