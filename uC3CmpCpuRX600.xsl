<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

  <!-- CPUエントリ --> 
  <xsl:template match="CPU">
    <p><hr/></p>
    <h3>[CPUデバイスの設定値]</h3>
    <h4>CPU</h4>
    <table>
    <tr>
    <th>Vendor</th>
    <th>型番</th>
    <th>ターゲット</th>
    </tr>
    <tr>
    <td><xsl:value-of select="chip/@vendor"/></td>
    <td><xsl:value-of select="chip/@name"/></td>
    <td><xsl:value-of select="chip/@target"/></td>
    </tr>

    </table>
    <h4>CPUコア</h4>
    <xsl:apply-templates select="Core" />
    <h4>CMTタイマ選択</h4>
    <xsl:apply-templates select="Core2" />
    <h4>クロック</h4>
    <xsl:apply-templates select="Clock" />
    <xsl:apply-templates select="UART" />
    <xsl:apply-templates select="Ethernet" />
  </xsl:template>

  <!-- Core -->
  <xsl:template match="Core">
    <table>
      <tr>
        <th>周期タイマにCMTを使用する</th>
        <th>周期タイマ割込みレベル</th>
        <th>低消費電力モード</th>
      </tr>
      <tr>
        <td>
          <xsl:choose>
            <xsl:when test="CFG_SYSTICK_USE=1">使用する</xsl:when>
            <xsl:when test="CFG_SYSTICK_USE='true'">使用する</xsl:when>
            <xsl:when test="CFG_SYSTICK_USE=0">使用しない</xsl:when>
            <xsl:when test="CFG_SYSTICK_USE='false'">使用しない</xsl:when>
          </xsl:choose>
        </td>
        <td><xsl:value-of select="CFG_SYSTICK_INTLVL"/></td>
        <td>
          <xsl:choose>
            <xsl:when test="CFG_SLEEP_MODE=1">有効</xsl:when>
            <xsl:when test="CFG_SLEEP_MODE='true'">有効</xsl:when>
            <xsl:when test="CFG_SLEEP_MODE=0">無効</xsl:when>
            <xsl:when test="CFG_SLEEP_MODE='false'">無効</xsl:when>
          </xsl:choose>
        </td>
      </tr>
    </table>

    <h4>例外ハンドラ</h4>
    <table>
      <tr>
        <th>例外番号</th>
        <th>例外タイプ</th>
        <th>起動番地</th>
      </tr>
    <xsl:apply-templates select="Exception" />
    </table>
  </xsl:template>

  <!-- 例外ハンドラ -->
  <xsl:template match="Exception">
    <tr>
      <td><xsl:value-of select="@no"/></td>
      <td><xsl:value-of select="@type"/></td>
      <td><xsl:value-of select="@hdr"/></td>
    </tr>
  </xsl:template>


  <!-- Core -->
  <xsl:template match="Core2">

    <table>
      <tr>
        <th>CMTタイマ番号</th>
        <th>選択:1 非選択:0</th>
      </tr>
    <xsl:apply-templates select="Systimers" />
    </table>
  </xsl:template>

  <!-- 例外ハンドラ -->
  <xsl:template match="Systimers">
    <tr>
      <td><xsl:value-of select="@tim_name"/></td>
      <td><xsl:value-of select="@tim_use"/></td>
    </tr>
  </xsl:template>







  <!-- クロック -->
  <xsl:template match="Clock">
    <table>
    <tr>
    <th>メイン発振回路</th>
    <th>PLL DIV</th>
    <th>PLL STC</th>
    <th>ソースクロック</th>
    <th>ICLK 分周</th>
    <th>PCLK 分周</th>
    <th>BCLK 分周</th>
    <th>UCLK 分周</th>
    <th>BCLK端子設定</th>
    <th>SDCLK端子設定</th>
    </tr>
    <tr>
    <td><xsl:value-of select="CFG_RX600_CLKMO"/>Hz</td>
    <td><xsl:value-of select="CFG_RX600_PLLDIV"/></td>
    <td><xsl:value-of select="CFG_RX600_STC"/></td>
    <td><xsl:value-of select="CFG_RX600_SCK"/></td>
    <td><xsl:value-of select="CFG_RX600_ICLK"/></td>
    <td><xsl:value-of select="CFG_RX600_PCLKB"/></td>
    <td><xsl:value-of select="CFG_RX600_BCLK"/></td>
    <td><xsl:value-of select="CFG_RX600_UCLK"/></td>
    <td><xsl:value-of select="CFG_RX600_BCLKDIV"/></td>
    <td><xsl:value-of select="CFG_RX600_SDCLK"/></td>
    </tr>
    </table>
  </xsl:template>

  <!-- UART -->
  <xsl:template match="UART">
    <h4><xsl:value-of select="@name"/></h4>
    <table>
    <tr>
    <th>使用</th>
    <th>デバイスID</th>
    <th>割込みレベル</th>
    <th>送信バッファサイズ</th>
    <th>受信バッファサイズ</th>
    <th>XOFF送信レベル</th>
    <th>XON送信レベル</th>
    <th>リロケート端子選択</th>
    </tr>
    <tr>
    <td>
    <xsl:choose>
      <xsl:when test="@enable=1">する</xsl:when>
      <xsl:when test="@enable='true'">する</xsl:when>
      <xsl:when test="@enable=0">しない</xsl:when>
      <xsl:when test="@enable='false'">しない</xsl:when>
    </xsl:choose>
    </td>
    <td><xsl:value-of select="CFG_RX600_UARTID"/></td>
    <td><xsl:value-of select="CFG_RX600_UART_INTLVL"/></td>
    <td><xsl:value-of select="CFG_RX600_UART_TXBUFSZ"/></td>
    <td><xsl:value-of select="CFG_RX600_UART_RXBUFSZ"/></td>
    <td><xsl:value-of select="CFG_RX600_UART_XOFF"/></td>
    <td><xsl:value-of select="CFG_RX600_UART_XON"/></td>
    <td><xsl:value-of select="CFG_RX600_UART_RELOCATE"/></td>
    <td>
    <xsl:choose>
      <xsl:when test="CFG_MB9BF_UART_HARDFLOW=1">使用する</xsl:when>
      <xsl:when test="CFG_MB9BF_UART_HARDFLOW='true'">使用する</xsl:when>
      <xsl:when test="CFG_MB9BF_UART_HARDFLOW=0">使用しない</xsl:when>
      <xsl:when test="CFG_MB9BF_UART_HARDFLOW='false'">使用しない</xsl:when>
    </xsl:choose>
    </td>
    </tr>
    </table>
  </xsl:template>

  <!-- Ethernet -->
  <xsl:template match="Ethernet">
    <h4><xsl:value-of select="@name"/></h4>
    <table>
    <tr>
    <th>使用</th>
    <th>割込みレベル</th>
    <th>PHY ID</th>
 <!--    <th>MIIモード</th>  -->
    <th>PHYモード</th>
    <th>FILTERモード</th>
 <!--    <th>Checksum offload</th>  -->
    <th>TX Buffer数</th>
    <th>RX Buffer数</th>
    <th>BUFFER SIZE</th>
    </tr>
    <tr>
    <td>
    <xsl:choose>
      <xsl:when test="@enable=1">する</xsl:when>
      <xsl:when test="@enable='true'">する</xsl:when>
      <xsl:when test="@enable=0">しない</xsl:when>
      <xsl:when test="@enable='false'">しない</xsl:when>
    </xsl:choose>
    </td>
    <td><xsl:value-of select="CFG_RX600_ETHER_INTLVL"/></td>
    <td><xsl:value-of select="CFG_RX600_ETHER_PHYID"/></td>
  <!--   <td> -->
  <!--   <xsl:if test="CFG_RX600_ETHER_MII=0">MII</xsl:if> -->
  <!--   <xsl:if test="CFG_RX600_ETHER_MII=1">RMII</xsl:if> -->
  <!--   </td> -->
    <td>
    <xsl:if test="CFG_RX600_ETHER_PHYMODE=0">自動</xsl:if>
    <xsl:if test="CFG_RX600_ETHER_PHYMODE=1">10Mbps ハーフデュプレックスモード</xsl:if>
    <xsl:if test="CFG_RX600_ETHER_PHYMODE=2">10Mbps フル/ハーフデュプレック自動選択モード</xsl:if>
    <xsl:if test="CFG_RX600_ETHER_PHYMODE=3">100Mbps ハーフデュプレックスモード</xsl:if>
    <xsl:if test="CFG_RX600_ETHER_PHYMODE=4">100Mbps フル/ハーフデュプレック自動選択モード</xsl:if>
    </td>
    <td>
    <xsl:if test="CFG_RX600_ETHER_FILTERMODE=0">Perfect filter</xsl:if>
    <xsl:if test="CFG_RX600_ETHER_FILTERMODE=1">Promiscuous</xsl:if>
    <xsl:if test="CFG_RX600_ETHER_FILTERMODE=2">Multicast</xsl:if>
    </td>
  <!--   <td> -->
  <!--    <xsl:if test="CFG_RX600_ETHER_CHECKSUM=0">使用しない</xsl:if> -->
  <!--    <xsl:if test="CFG_RX600_ETHER_CHECKSUM=1">Txのみ使用</xsl:if> -->
  <!--    <xsl:if test="CFG_RX600_ETHER_CHECKSUM=2">Rxのみ使用</xsl:if> -->
  <!--    <xsl:if test="CFG_RX600_ETHER_CHECKSUM=3">両方使用</xsl:if> -->
  <!--    </td>  -->
    <td><xsl:value-of select="CFG_RX600_ETHER_TXBUFFER"/></td>
    <td><xsl:value-of select="CFG_RX600_ETHER_RXBUFFER"/></td>
    <td><xsl:value-of select="CFG_RX600_ETHER_BUFFERSIZE"/></td>
    </tr>
    </table>
  </xsl:template>
  
</xsl:stylesheet>
