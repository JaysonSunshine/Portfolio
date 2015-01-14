<!DOCTYPE html>
<html>
<body>

<h1>Add actor or director</h1>

<form action="add_actor_director.php" method="GET">
Identity: <input type = "radio" name="identity" value="Actor">Actor
<input type = "radio" name="identity" value="Director">Director<br>
First name: <input type = "text" name="firstname"><br>
Last name: <input type = "text" name="lastname"><br>
Sex: <input type = "radio" name="sex" value="male">Male
	 <input type = "radio" name="sex" value="female">Female<br>
Date of Birth: <input type = "text" name="birth">(eg 19450217)<br>
Date of Death: <input type = "text" name="death">(optional)<br>
<input type="submit" value="Submit">
</form>

<?php
	if(!empty($_GET)){
		$db_connection = mysqli_connect("localhost", "cs143", "", "CS143"); #opens connection to database
		if(mysqli_connect_errno()){
			print "no connect";
		}

		$db_connection_1 = mysql_connect("localhost", "cs143", ""); #opens connection to database
		mysql_select_db("CS143", $db_connection_1); #opens CS143 database

		$query = "select * from MaxPersonID";
		$rs = mysql_query($query, $db_connection_1); #results of query
		if($rs != FALSE){
			$meta = mysql_fetch_row($rs);

			$_GET[birth] = str_replace('/', '', $_GET[birth]);
			if($_GET[firstname] == NULL){
				$_GET[firstname] = 'NULL';
			}
			if($_GET[lastname] == NULL){
				$_GET[lastname] = 'NULL';
			}
			if($_GET[birth] == NULL){
				$_GET[birth] = 'NULL';
			}
			if($_GET[death] == NULL){
				$_GET[death] = 'NULL';
			}
			if($_GET[identity] == Director){
				$sql = "INSERT INTO $_GET[identity] VALUES($meta[0] + 1, '$_GET[lastname]', '$_GET[firstname]', $_GET[birth], $_GET[death])";
			}
			else{
				$sql = "INSERT INTO $_GET[identity] VALUES($meta[0] + 1, '$_GET[lastname]', '$_GET[firstname]', '$_GET[sex]', $_GET[birth], $_GET[death])";
			}
			if(!mysqli_query($db_connection, $sql)){
				print "failed";
			}
			else{
				$sql = " UPDATE MaxPersonID SET id = $meta[0] + 1 WHERE id = $meta[0]";
				mysqli_query($db_connection, $sql);
				if($_GET[identity] == Actor){
					print "<br>Actor added!";
				}
				else{
					print "<br>Director added!";
				}
			}
		}
	}
?>

<br><br><a href="search.php">Search Database</a> 

<br><br><a href="query.php">Return home</a> 

</body>
</html>