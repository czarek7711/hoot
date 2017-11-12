<?xml version="1.0" encoding="Shift_JIS"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/TR/WD-xsl" xml:lang="ja">
<xsl:template match="/">
	<html lang="ja">
	<head>
		<title>hoot ドライバリスト</title>
		<meta http-equiv="Content-Type" content="text/html; charset=Shift_JIS"/>
		<link rel="stylesheet" type="text/css" href="hoot.css"/>
	</head>
	<body>
		<h1>hoot ドライバリスト</h1>
		<hr/>
		<xsl:apply-templates/>
	</body>
	</html>
</xsl:template>
<xsl:template match="drivers">
	<xsl:apply-templates/>
</xsl:template>
<xsl:template match="driver">
	<h2><xsl:value-of select="name"/></h2>
	<xsl:apply-templates/>
</xsl:template>
<xsl:template match="subtypes">
	<xsl:apply-templates/>
</xsl:template>
<xsl:template match="subtype">
	<h3><xsl:value-of select="name"/></h3>
	<xsl:apply-templates/>
	<hr/>
</xsl:template>
<xsl:template match="descriptions">
	<xsl:apply-templates/>
</xsl:template>
<xsl:template match="description">
	<xsl:value-of/><br/>
</xsl:template>
<xsl:template match="files">
	<h4>Files</h4>
	<table>
		<tr>
			<th width="20%">type</th>
			<th align="left">descriptions</th>
		</tr>
		<xsl:apply-templates/>
	</table>
</xsl:template>
<xsl:template match="file">
	<tr>
		<th width="20%"><xsl:value-of select="name"/></th>
		<td><xsl:apply-templates/></td>
	</tr>
</xsl:template>
<xsl:template match="options">
	<h4>Options</h4>
	<table>
		<tr>
			<th width="20%">name</th>
			<th width="15%">default</th>
			<th align="left">descriptions</th>
		</tr>
		<xsl:apply-templates/>
	</table>
</xsl:template>
<xsl:template match="option">
	<tr>
		<th width="20%"><xsl:value-of select="name"/></th>
		<th width="15%"><xsl:value-of select="value"/></th>
		<td><xsl:apply-templates/></td>
	</tr>
</xsl:template>
</xsl:stylesheet>
