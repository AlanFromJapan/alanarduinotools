<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:template match="/">
  <html>
	<head>
		<link rel="stylesheet" type="text/css" href="Styles.css" />
	</head>
  <body>
  <h2>Errors list</h2>
	<div class="generationDetails">Generation duration : <xsl:value-of select="logfileExport/generationDetails/generationDuration"/></div>
    <table class="mainTable">
      <tr bgcolor="#9acd32">
        <th style="text-align:left">#</th>
        <th style="text-align:left">Time</th>
        <th style="text-align:left">Bip <i>(running count)</i></th>
        <th style="text-align:left">Title</th>
      </tr>
      <xsl:for-each select="logfileExport/logItems/logItem">
      <tr>
		<xsl:attribute name="class">
			<xsl:value-of select="wellKnownErrorPriority" />
		</xsl:attribute>
  
		<td rowspan="2" class="lineNumberStyle"><xsl:value-of select="line"/></td>
        <td class="timeStyle"><xsl:value-of select="time"/></td>
        <td><xsl:value-of select="bipcode"/> <i style="font-size:smaller;">(<xsl:value-of select="bipcount"/>)</i></td>
        <td>
			<xsl:value-of select="msgTitle"/>
			<xsl:if test="wellKnownErrorLabel != ''">
				<br/>
				<div class="hintStyle"><img src="hint.gif"/><xsl:value-of select="wellKnownErrorLabel"/></div>
			</xsl:if>
		</td>
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

