<!DOCTYPE html>
<html>
<body>

<?php
	if(!empty($_GET)){
		$db_connection = mysqli_connect("localhost", "cs143", "", "CS143"); #opens connection to database
		if(mysqli_connect_errno()){
			print "Failed to connect to database";
		}
		else{
			$query = "select * from Actor where id = $_GET[id]";
			$rs = mysqli_query($db_connection, $query); #results of query
			if($rs != FALSE){

				$row = mysqli_fetch_row($rs);
				print "<h1>$row[2] $row[1]</h1>";
				print "Sex: $row[3] <br> Date of Birth: $row[4]";
				if($row[5] != NULL){
					print "<br>Date of Death: $row[5]";
				}
			}
			$query = "select * from MovieActor where aid = $_GET[id]";
			$rs = mysqli_query($db_connection, $query); #results of query
			print "<h2>Roles</h2>";
			if($rs != FALSE){
				while($row = mysqli_fetch_row($rs)) {
					$query_movie = "select title, id, year from Movie where id = $row[0]";
					$result = mysqli_query($db_connection, $query_movie); #results of query
					$row_movie = mysqli_fetch_row($result);
					print "$row[2] in <a href=" . '"' . "browse_movie.php?id=$row_movie[1]". '"">' . "$row_movie[0] [$row_movie[2]]</a><br>";
				}
			}
		}
	}
	else{
		print "<h1>Browse Actors</h1>";
	}
?>
<br>
<form action="search.php" method="GET">
Search: <input type = "text" name="search"><br>
<input type="submit" value="Submit">
</form>	
<br><a href="query.php">Movie Database</a> 


</body>
</html>