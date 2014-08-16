<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:template match="/">
  <html>
	<head>
		<link rel="stylesheet" type="text/css" href="Styles.css" />
	</head>
  <body>
  <h2>Errors list</h2>
    <table border="1">
      <tr bgcolor="#9acd32">
        <th style="text-align:left">Time</th>
        <th style="text-align:left">Bip (<i>running count</i>)</th>
        <th style="text-align:left">Title</th>
      </tr>
      <xsl:for-each select="logItems/logItem">
      <tr>
		<xsl:attribute name="class">
			<xsl:value-of select="prioStyle" />
		</xsl:attribute>
  
        <td><xsl:value-of select="time"/></td>
        <td><xsl:value-of select="bipcode"/> (<i><xsl:value-of select="bipcount"/></i>)</td>
        <td><xsl:value-of select="msgTitle"/></td>
      </tr>	
	  <tr>
		<td colspan="3" class="rawContent"><xsl:value-of select="rawContent"/></td>
	  </tr>
      </xsl:for-each>
    </table>
  </body>
  </html>
</xsl:template>
</xsl:stylesheet>

