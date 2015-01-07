<!DOCTYPE html>
<html>
<body>

<h1>Add comments to a movie</h1>

<form action="add_comments.php" method="GET">
Name: <input type = "text" name="name"><br>
Movie: <input type = "text" name="movie"><br>
Rating: <input type = "radio" name="rating" value=1>1
	    <input type = "radio" name="rating" value=2>2
	    <input type = "radio" name="rating" value=3>3
	 	<input type = "radio" name="rating" value=4>4
	 	<input type = "radio" name="rating" value=5>5<br>
<textarea name="comment" cols="50" rows="10"></textarea>
<input type="submit" value="Submit">
</form>

<?php
	if(($_GET[rating] and $_GET[movie]) != NULL){
		$db_connection = mysqli_connect("localhost", "cs143", "", "CS143"); #opens connection to database
		if(mysqli_connect_errno()){
			print "no connect";
		}
		$db_connection_1 = mysql_connect("localhost", "cs143", ""); #opens connection to database
		mysql_select_db("CS143", $db_connection_1); #opens CS143 database

		$query = "select id from Movie where title = '$_GET[movie]'";

		$rs = mysql_query($query, $db_connection_1); #results of query
		if($rs != FALSE){

			$meta = mysql_fetch_row($rs);
			if($_GET[name] == NULL){
				$_GET[name] = 'NULL';
			}
			if($_GET[comment] == NULL){
				$_GET[comment] = 'NULL';
			}
			if($meta[0] != NULL){
				$sql = "INSERT INTO Review VALUES('$_GET[name]', CURRENT_TIMESTAMP, $meta[0],  $_GET[rating], '$_GET[comment]')";
				if(!mysqli_query($db_connection, $sql)){
					print "failed";
				}
				else{
					print "Review added!";
				}
			}
			else{
				print "Movie does not exist in database!";
			}
		}
	}
?>

<br><br><a href="query.php">Return home</a> 


</body>
</html>