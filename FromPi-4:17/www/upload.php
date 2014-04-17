<html>
<head>
	<title>File Upload Form</title>
</head>
<body>
	This form allows you to upload a file to the server.<br>
	<form action="getfile.php" method="post"><br>
		Type (or select) Filename: <input type="file" name="uploadFile">
		<input type="hidden" name="MAX_FILE_SIZE" value="10000000" />
		<input type="submit" value="Upload File">
	</form>
</body>
</html>
