<!DOCTYPE html>
<html>
<body>

<h1>Add movie information</h1>

<form action="add_movie.php" method="GET">
Title: <input type = "text" name="title"><br>
Year: <input type = "text" name="year"><br>
Company: <input type = "text" name="Company"><br>
Rating: <select name = "rating">
<?php
	$list = array("G", "PG", "PG-13", "R", "NC-17");
	for($i = 0; $i<5; $i++){
		print '<option value = "' . "$list[$i]" . '">' . "$list[$i]</option>";
	}
?>
</select> <br>
Genre: <br>
<?php
	$list = array("Action", "Adult", "Adventure", "Animation", "Comedy", "Crime", "Documentary", "Drama", "Family", "Fantasy", "Horror", "Musical",
	 			  "Mystery", "Romance", "Sci-Fi", "Short", "Thriller", "War", "Western");
	for($i = 0; $i < 19; $i++){
		print '<input type = "checkbox" name="genre[]" value="' . "$list[$i]" . '"/>' . "$list[$i]<br>";
	}
	print "<br>";
	#print $_GET["cars"];
?>
<!--<input type = "checkbox" name="death" value="Action">Action
<input type = "checkbox" name="death" value="Action">Action-->

<input type="submit" value="Submit">
</form>

<?php
	if(!empty($_GET)){
		$db_connection = mysqli_connect("localhost", "cs143", "", "CS143"); #opens connection to database
		if(mysqli_connect_errno()){
			print "Failed to connect to database";
		}
		else{
			if($_GET[rating] == NULL){
					$_GET[rating] = 'NULL';
			}
			if($_GET[company] == NULL){
					$_GET[company] = 'NULL';
			}
			$db_connection_1 = mysql_connect("localhost", "cs143", ""); #opens connection to database
			mysql_select_db("CS143", $db_connection_1); #opens CS143 database

			$query = "select * from MaxMovieID";
			$rs = mysql_query($query, $db_connection_1); #results of query
			if($rs != FALSE){
				$meta = mysql_fetch_row($rs);
			
				$sql = "INSERT INTO Movie VALUES($meta[0] + 1, '$_GET[title]', $_GET[year], '$_GET[rating]', '$_GET[company]')";
				if(!mysqli_query($db_connection, $sql)){
					print "failed to add movie";
				}
				else{
					$sql = " UPDATE MaxMovieID SET id = $meta[0] + 1 WHERE id = $meta[0]";
					mysqli_query($db_connection, $sql);
					print "Movie added!";
				}
			}
			$genre = $_GET[genre];
			for($i = 0; $i < count($genre); $i++){
				$sql = "INSERT INTO MovieGenre VALUES($meta[0] + 1, '$genre[$i]')";
				if(!mysqli_query($db_connection, $sql)){
					print "failed to add movie genre(s)";
				}
			}
		}
	}
?>


<br><br><a href="search.php">Search Database</a> <br>
<a href="add_actor_movie.php">Add actor to a movie</a> <br>
<a href="add_director_movie.php">Add director to a movie</a> <br><br> 


<a href="query.php">Return home</a> 


</body>
</html>