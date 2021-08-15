<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

  <xsl:template match="Middle-uNet3">
    <p><hr/></p>
    <h3>[uNet3の設定値]</h3>
    <h4>uNet3全般</h4>
    <xsl:apply-templates select="General" />

    <h4>ネットワークインタフェース</h4>
    <table>
    <tr>
    <th>IDの定義名</th>
    <th>デバイス名</th>
    <th>MTU</th>
    <th>MAC</th>
    <th>IPv4-DHCP使用</th>
    <th>IPv4-アドレス</th>
    <th>IPv4-サブネットマスク</th>
    <th>IPv4-デフォルトゲートウェイ</th>
    <th>IPv4-IPアドレスの重複チェック</th>
    <th>IPv6-静的アドレス使用</th>
    <th>IPv6-アドレス</th>
    <th>IPv6-プリフィックスの長さ</th>
    <th>IPv6-ルーターアドレス</th>
    </tr>
    <xsl:apply-templates select="NetIf" />
    </table>
    
    <h4>ネットワークインタフェース(PPP)</h4>
    <table>
    <tr>
    <th>IDの定義名</th>
    <th>デバイス名</th>
    <th>MTU</th>
    <th>ユーザ名</th>
    <th>パスワード</th>
    <th>ダイヤル</th>
    <th>COMデバイス</th>
    <th>ボーレート</th>
    <th>フロー制御</th>
    </tr>
    <xsl:apply-templates select="NetIf" mode="PPP_L1" />
    </table>
    
    <table>
    <tr>
    <th>IDの定義名</th>
    <th>IPアドレス取得</th>
    <th>ローカルIPアドレス</th>
    <th>リモートIPアドレス</th>
    <th>DNSアドレス取得</th>
    <th>プライマリDNSアドレス</th>
    <th>セカンダリDNSアドレス</th>
    <th>認証プロトコル</th>
    <th>VJ圧縮</th>
    <th>VJスロット数</th>
    <th>リトライ回数</th>
    <th>リトライ間隔(ms)</th>
    </tr>
    <xsl:apply-templates select="NetIf" mode="PPP_L2" />
    </table>
    
    <h4>ソケット</h4>
    <table>
    <tr>
    <th>IDの定義名</th>
    <th>インタフェースのバインディング</th>
    <th>IPバージョン</th>
    <th>プロトコル</th>
    <th>ローカルポート</th>
    <th>送信タイムアウト(ミリ秒)</th>
    <th>受信タイムアウト(ミリ秒)</th>
    <th>接続タイムアウト(ミリ秒)</th>
    <th>切断タイムアウト(ミリ秒)</th>
    <th>送信バッファ(byte)</th>
    <th>受信バッファ(byte)</th>
    </tr>
    <xsl:apply-templates select="Socket" />
    </table>
    
    <h4>ネットアプリケーション(HTTPd)</h4>
    <table>
    <tr>
    <th>使用</th>
    <th>セッション数</th>
    </tr>
    <xsl:apply-templates select="NetApplication/HTTPd" />
    </table>
    <!-- contents リスト -->
    <table>
    <tr>
    <th>Content-Type</th>
    <th>URL</th>
    <th>Resource</th>
    </tr>
    <xsl:apply-templates select="NetApplication/HTTPd/Content" />
    </table>
    
    <h4>ネットアプリケーション(DHCP Client)</h4>
    <table>
    <tr>
    <th>拡張機能の使用</th>
    <th>リトライ回数</th>
    </tr>
    <xsl:apply-templates select="NetApplication/DHCPClient" />
    </table>

    <h4>ネットアプリケーション(PING)</h4>
    <table>
    <tr>
    <th>ICMP Echo要求の使用</th>
    </tr>
    <xsl:apply-templates select="NetApplication/Ping" />
    </table>

    <h4>SSL</h4>
    <table>
    <tr>
      <th>SSLの使用</th>
      <th>SSLのバージョン</th>
      <th>最大受信サイズ</th>
      <th>最大受信チェーン数</th>
      <th>ssl_cli_iniで使用するクライアントの証明書の最大数</th>
      <th>最大同時セッション数</th>
      <th>最大同時コネクション数</th>
      <th>内部バッファのサイズ</th>
      <th>内部バッファの数</th>
      <th>RC4 MD5</th>
      <th>RC4 SHA1</th>
      <th>TDES</th>
      <th>AES-128</th>
      <th>AES-256</th>
      <th>暗号用ドライバのタイムアウト</th>
      <th>HASH用ドライバのタイムアウト</th>
      <th>優先度の異なる複数のタスクからセッションの確立を発行する</th>
    </tr>
      <xsl:apply-templates select="SSL" />
    </table>

    </xsl:template>

  <!-- 全般 -->
  <xsl:template match="Middle-uNet3/General">
    <table>
      <tr>
        <th>uNet3の使用</th>
        <th>uNet3を起動するタスク(ID)</th>
        <th>簡単設定モード</th>
        <th>詳細設定の使用</th>
      </tr>
      <tr>
        <td>
        <xsl:choose>
          <xsl:when test="CFG_UNET3_USE=1">する</xsl:when>
          <xsl:when test="CFG_UNET3_USE='true'">する</xsl:when>
          <xsl:when test="CFG_UNET3_USE=0">しない</xsl:when>
          <xsl:when test="CFG_UNET3_USE='false'">しない</xsl:when>
        </xsl:choose>
        </td>
        <td><xsl:value-of select="CFG_UNET3_INITTSK"/></td>
        <td><xsl:value-of select="CFG_UNET3_PARAM_MODE"/></td>
        <td>
        <xsl:choose>
          <xsl:when test="CFG_UNET3_PARAM_CUSTOMIZE=1">する</xsl:when>
          <xsl:when test="CFG_UNET3_PARAM_CUSTOMIZE='true'">する</xsl:when>
          <xsl:when test="CFG_UNET3_PARAM_CUSTOMIZE=0">しない</xsl:when>
          <xsl:when test="CFG_UNET3_PARAM_CUSTOMIZE='false'">しない</xsl:when>
        </xsl:choose>
        </td>
      </tr>
    </table>
    <h4>基本設定</h4>
    <xsl:apply-templates select="Basic" />
    <h4>IP設定</h4>
    <xsl:apply-templates select="IP" />
    <h4>ARP設定</h4>
    <xsl:apply-templates select="ARP" />
    <h4>TCP設定</h4>
    <xsl:apply-templates select="TCP" />
    <h4>UDP設定</h4>
    <xsl:apply-templates select="UDP" />
    <h4>IPv6設定</h4>
    <xsl:apply-templates select="IPv6" />
  </xsl:template>

  <!-- 基本設定 -->
  <xsl:template match="Middle-uNet3/General/Basic">
    <table>
      <tr>
        <th>ネットワークバッファ数</th>
        <th>ARPキャッシュ</th>
        <th>受信フラグメントパケット</th>
        <th>マルチキャスト参加グループ</th>
      </tr>
      <tr>
        <td><xsl:value-of select="CFG_UNET3_NETBUF_CNT"/></td>
        <td><xsl:value-of select="CFG_UNET3_ARPMAX"/></td>
        <td><xsl:value-of select="CFG_UNET3_IPRMAX"/></td>
        <td><xsl:value-of select="CFG_UNET3_MGRMAX"/></td>
      </tr>
    </table>
  </xsl:template>
  
  <!-- IP設定 -->
  <xsl:template match="Middle-uNet3/General/IP">
    <table>
      <tr>
        <th>TTL</th>
        <th>TOS</th>
        <th>IPフラグメントパケット待ち時間(秒)</th>
        <th>受信パケットのチェックサムを無視</th>
      </tr>
      <tr>
        <td><xsl:value-of select="CFG_UNET3_IP4_TTL"/></td>
        <td><xsl:value-of select="CFG_UNET3_IP4_TOS"/></td>
        <td><xsl:value-of select="CFG_UNET3_IP4_IPR_TMO"/></td>
        <td>
        <xsl:choose>
          <xsl:when test="CFG_UNET3_IP4_IGNORE_CHECKSUM=1">する</xsl:when>
          <xsl:when test="CFG_UNET3_IP4_IGNORE_CHECKSUM='true'">する</xsl:when>
          <xsl:when test="CFG_UNET3_IP4_IGNORE_CHECKSUM=0">しない</xsl:when>
          <xsl:when test="CFG_UNET3_IP4_IGNORE_CHECKSUM='false'">しない</xsl:when>
        </xsl:choose>
        </td>
      </tr>
    </table>
  </xsl:template>

  <!-- ARP設定 -->
  <xsl:template match="Middle-uNet3/General/ARP">
    <table>
      <tr>
        <th>リトライ回数</th>
        <th>リトライ間隔(秒)</th>
        <th>IPキャッシュ有効期間(分)</th>
      </tr>
      <tr>
        <td><xsl:value-of select="CFG_UNET3_ARP_RET_CNT"/></td>
        <td><xsl:value-of select="CFG_UNET3_ARP_RET_TMO"/></td>
        <td><xsl:value-of select="CFG_UNET3_ARP_CLR_TMO"/></td>
      </tr>
    </table>
  </xsl:template>

  <!-- TCP設定 -->
  <xsl:template match="Middle-uNet3/General/TCP">
    <table>
      <tr>
        <th>SYNタイムアウト(秒)</th>
        <th>再送タイムアウト(秒)</th>
        <th>IP切断タイムアウト(秒)</th>
        <th>受信パケットのチェックサムを無視</th>
        <th>KeepAliveの通知回数</th>
        <th>KeepAliveの起動時間(秒)</th>
        <th>KeepAliveの通知間隔(秒)</th>
      </tr>
      <tr>
        <td><xsl:value-of select="CFG_UNET3_TCP_CON_TMO"/></td>
        <td><xsl:value-of select="CFG_UNET3_TCP_SND_TMO"/></td>
        <td><xsl:value-of select="CFG_UNET3_TCP_CLS_TMO"/></td>
        <td>
        <xsl:choose>
          <xsl:when test="CFG_UNET3_TCP_IGNORE_CHECKSUM=1">する</xsl:when>
          <xsl:when test="CFG_UNET3_TCP_IGNORE_CHECKSUM='true'">する</xsl:when>
          <xsl:when test="CFG_UNET3_TCP_IGNORE_CHECKSUM=0">しない</xsl:when>
          <xsl:when test="CFG_UNET3_TCP_IGNORE_CHECKSUM='false'">しない</xsl:when>
        </xsl:choose>
        </td>
        <td><xsl:value-of select="CFG_UNET3_TCP_KPA_CNT"/></td>
        <td><xsl:value-of select="CFG_UNET3_TCP_KPA_TMO"/></td>
        <td><xsl:value-of select="CFG_UNET3_TCP_KPA_INT"/></td>
      </tr>
    </table>
  </xsl:template>

  <!-- UDP設定 -->
  <xsl:template match="Middle-uNet3/General/UDP">
    <table>
      <tr>
        <th>受信キューサイズ</th>
        <th>受信パケットのチェックサムを無視</th>
        <th>未使用ポート宛てのパケット受信時にICMP(Port unreachable)を送信</th>
      </tr>
      <tr>
        <td><xsl:value-of select="CFG_UNET3_PKT_RCV_QUE"/></td>
        <td>
        <xsl:choose>
          <xsl:when test="CFG_UNET3_UDP_IGNORE_CHECKSUM=1">する</xsl:when>
          <xsl:when test="CFG_UNET3_UDP_IGNORE_CHECKSUM='true'">する</xsl:when>
          <xsl:when test="CFG_UNET3_UDP_IGNORE_CHECKSUM=0">しない</xsl:when>
          <xsl:when test="CFG_UNET3_UDP_IGNORE_CHECKSUM='false'">しない</xsl:when>
        </xsl:choose>
        </td>
        <td>
        <xsl:choose>
          <xsl:when test="CFG_UNET3_PKT_UNREACH=1">する</xsl:when>
          <xsl:when test="CFG_UNET3_PKT_UNREACH='true'">する</xsl:when>
          <xsl:when test="CFG_UNET3_PKT_UNREACH=0">しない</xsl:when>
          <xsl:when test="CFG_UNET3_PKT_UNREACH='false'">しない</xsl:when>
        </xsl:choose>
        </td>
      </tr>
    </table>
  </xsl:template>

  <!-- IPv6設定 -->
  <xsl:template match="Middle-uNet3/General/IPv6">
    <table>
      <tr>
        <th>IPv6終端キャッシュ</th>
        <th>IPv6近隣キャッシュ</th>
        <th>IPv6プレフィックスリスト</th>
        <th>IPv6デフォルトルータリスト</th>
        <th>IPv6パスMTUキャッシュ</th>
      </tr>
      <tr>
        <td><xsl:value-of select="CFG_UNET3_DST_CACHE"/></td>
        <td><xsl:value-of select="CFG_UNET3_NEIGH_CACHE"/></td>
        <td><xsl:value-of select="CFG_UNET3_PRFX_LST"/></td>
        <td><xsl:value-of select="CFG_UNET3_RTR_LST"/></td>
        <td><xsl:value-of select="CFG_UNET3_PMTU_CACHE"/></td>
      </tr>
    </table>
  </xsl:template>
  
  <!-- ネットワークインタフェース -->
  <xsl:template match="NetIf">
  <xsl:if test="not(contains(CFG_UNET3_IFDEV, 'PPP'))">
    <tr>
      <td><xsl:value-of select="@CFG_UNET3_IFID"/></td>
      <td><xsl:value-of select="CFG_UNET3_IFDEV"/></td>
      <td><xsl:value-of select="CFG_UNET3_IFMTU"/></td>
      <td><xsl:value-of select="CFG_UNET3_IFMAC"/></td>
      <td>
      <xsl:choose>
        <xsl:when test="IPv4/CFG_UNET3_IF_DHCP_USE=1">する</xsl:when>
        <xsl:when test="IPv4/CFG_UNET3_IF_DHCP_USE='true'">する</xsl:when>
        <xsl:when test="IPv4/CFG_UNET3_IF_DHCP_USE=0">しない</xsl:when>
        <xsl:when test="IPv4/CFG_UNET3_IF_DHCP_USE='false'">しない</xsl:when>
      </xsl:choose>
      </td>
      <td><xsl:value-of select="IPv4/CFG_UNET3_IF_IPADDR"/></td>
      <td><xsl:value-of select="IPv4/CFG_UNET3_IF_SUBNET"/></td>
      <td><xsl:value-of select="IPv4/CFG_UNET3_IF_GATEWAY"/></td>
      <td>
      <xsl:choose>
        <xsl:when test="IPv4/CFG_UNET3_IF_CHECKDUP=1">する</xsl:when>
        <xsl:when test="IPv4/CFG_UNET3_IF_CHECKDUP='true'">する</xsl:when>
        <xsl:when test="IPv4/CFG_UNET3_IF_CHECKDUP=0">しない</xsl:when>
        <xsl:when test="IPv4/CFG_UNET3_IF_CHECKDUP='false'">しない</xsl:when>
      </xsl:choose>
      </td>
      <td>
      <xsl:choose>
        <xsl:when test="IPv6/CFG_UNET3_IF_IP6_STATIC=1">する</xsl:when>
        <xsl:when test="IPv6/CFG_UNET3_IF_IP6_STATIC='true'">する</xsl:when>
        <xsl:when test="IPv6/CFG_UNET3_IF_IP6_STATIC=0">しない</xsl:when>
        <xsl:when test="IPv6/CFG_UNET3_IF_IP6_STATIC='false'">しない</xsl:when>
      </xsl:choose>
      </td>
      <td><xsl:value-of select="IPv6/CFG_UNET3_IF_IP6_ADDR"/></td>
      <td><xsl:value-of select="IPv6/CFG_UNET3_IF_IP6_PREFIXLEN"/></td>
      <td><xsl:value-of select="IPv6/CFG_UNET3_IF_IP6_ROUTER"/></td>
    </tr>
  </xsl:if>
  </xsl:template>
  
  <!-- PPP設定 -->
  <xsl:template match="NetIf" mode="PPP_L1">
    <xsl:if test="contains(CFG_UNET3_IFDEV, 'PPP')">
      <tr>
        <td><xsl:value-of select="@CFG_UNET3_IFID"/></td>
        <td><xsl:value-of select="CFG_UNET3_IFDEV"/></td>
        <td><xsl:value-of select="CFG_UNET3_IFMTU"/></td>
        <td><xsl:value-of select="CFG_UNET3_PPP_USERNAME"/></td>
        <td><xsl:value-of select="CFG_UNET3_PPP_PASSWORD"/></td>
        <td><xsl:value-of select="CFG_UNET3_PPP_DIAL"/></td>
        <td><xsl:value-of select="CFG_UNET3_PPP_MODEM_DEVID"/></td>
        <td><xsl:value-of select="CFG_UNET3_PPP_MODEM_BAUD"/></td>
        <td><xsl:choose>
          <xsl:when test="CFG_UNET3_PPP_MODEM_FLOW=0">None</xsl:when>
          <xsl:when test="CFG_UNET3_PPP_MODEM_FLOW=1">Xon</xsl:when>
          <xsl:when test="CFG_UNET3_PPP_MODEM_FLOW=2">Hardware</xsl:when>
        </xsl:choose></td>
      </tr>
    </xsl:if>
  </xsl:template>
  <xsl:template match="NetIf" mode="PPP_L2">
    <xsl:if test="contains(CFG_UNET3_IFDEV, 'PPP')">
      <tr>
        <td><xsl:value-of select="@CFG_UNET3_IFID"/></td>
        <td><xsl:choose>
          <xsl:when test="CFG_UNET3_PPP_AUTO_IP=0">自動取得</xsl:when>
          <xsl:otherwise>手動設定</xsl:otherwise>
        </xsl:choose></td>
        <td><xsl:value-of select="CFG_UNET3_PPP_IP_LOCAL"/></td>
        <td><xsl:value-of select="CFG_UNET3_PPP_IP_REMOTE"/></td>
        <td><xsl:choose>
          <xsl:when test="CFG_UNET3_PPP_AUTO_DNS=0">自動取得</xsl:when>
          <xsl:otherwise>手動設定</xsl:otherwise>
        </xsl:choose></td>
        <td><xsl:value-of select="CFG_UNET3_PPP_DNS_PRIMARY"/></td>
        <td><xsl:value-of select="CFG_UNET3_PPP_DNS_SECONDARY"/></td>
        <td><xsl:choose>
          <xsl:when test="CFG_UNET3_PPP_AUTH_PROTO=0">CHAP</xsl:when>
          <xsl:otherwise>PAP</xsl:otherwise>
        </xsl:choose></td>
        <td><xsl:choose>
          <xsl:when test="CFG_UNET3_PPP_VJ_COMPRESS=0">する</xsl:when>
          <xsl:otherwise>しない</xsl:otherwise>
        </xsl:choose></td>
        <td><xsl:value-of select="CFG_UNET3_PPP_VJ_SLOTS"/></td>
        <td><xsl:value-of select="CFG_UNET3_PPP_RETRY_CNT"/></td>
        <td><xsl:value-of select="CFG_UNET3_PPP_RETRY_INTERVAL"/></td>
      </tr>
    </xsl:if>
  </xsl:template>
  
  <!-- ソケット -->
  <xsl:template match="Socket">
    <tr>
      <td><xsl:value-of select="@CFG_UNET3_SOCID"/></td>
      <td><xsl:value-of select="CFG_UNET3_SOC_BINDIF"/></td>
      <td>
      <xsl:choose>
        <xsl:when test="CFG_UNET3_SOC_IPVER=0">IPv4</xsl:when>
        <xsl:when test="CFG_UNET3_SOC_IPVER=1">IPv6</xsl:when>
      </xsl:choose>
      </td>
      <td>
      <xsl:choose>
        <xsl:when test="CFG_UNET3_SOCPROTO=6">TCP</xsl:when>
        <xsl:when test="CFG_UNET3_SOCPROTO=17">UDP</xsl:when>
        <xsl:when test="CFG_UNET3_SOCPROTO=1">ICMP</xsl:when>
        <xsl:when test="CFG_UNET3_SOCPROTO=58">ICMP6</xsl:when>
      </xsl:choose>
      </td>
      <td><xsl:value-of select="CFG_UNET3_SOCPORT"/></td>
      <td><xsl:value-of select="CFG_UNET3_SOC_SNDTMO"/></td>
      <td><xsl:value-of select="CFG_UNET3_SOC_RCVTMO"/></td>
      <td><xsl:value-of select="CFG_UNET3_SOC_CONTMO"/></td>
      <td><xsl:value-of select="CFG_UNET3_SOC_CLSTMO"/></td>
      <td><xsl:value-of select="CFG_UNET3_SOC_SNDBUF"/></td>
      <td><xsl:value-of select="CFG_UNET3_SOC_RCVBUF"/></td>
    </tr>
  </xsl:template>

  <!-- HTTPd -->
  <xsl:template match="NetApplication/HTTPd">
    <tr>
      <td>
      <xsl:choose>
        <xsl:when test="@enable='true'">する</xsl:when>
        <xsl:when test="@enable='false'">しない</xsl:when>
      </xsl:choose>
      </td>
      <td><xsl:value-of select="CFG_UNET3_HTTPD_SESSION"/></td>
    </tr>
  </xsl:template>
  <!-- コンテンツ一覧 -->
  <xsl:template match="NetApplication/HTTPd/Content">
    <tr>
    <td><xsl:value-of select="CFG_UNET3_HTTPD_CONTENT_TYPE"/></td>
    <td><xsl:value-of select="CFG_UNET3_HTTPD_CONTENT_URL"/></td>
    <td><xsl:value-of select="CFG_UNET3_HTTPD_CONTENT_RES"/></td>
    </tr>
  </xsl:template>

  <!-- DHCPClient -->
  <xsl:template match="NetApplication/DHCPClient">
    <tr>
      <td>
      <xsl:choose>
        <xsl:when test="CFG_UNET3_DHCPCLI_EXT_USE='true'">する</xsl:when>
        <xsl:when test="CFG_UNET3_DHCPCLI_EXT_USE='false'">しない</xsl:when>
      </xsl:choose>
      </td>
      <td><xsl:value-of select="CFG_UNET3_DHCPCLI_RETRY_CNT"/></td>
    </tr>
  </xsl:template>
  
  <!-- DHCPClient -->
  <xsl:template match="NetApplication/Ping">
    <tr>
      <td>
      <xsl:choose>
        <xsl:when test="CFG_UNET3_PING_ICMP_ECHO_USE='true'">する</xsl:when>
        <xsl:when test="CFG_UNET3_PING_ICMP_ECHO_USE='false'">しない</xsl:when>
      </xsl:choose>
      </td>
    </tr>
  </xsl:template>

  <!-- SSL -->
  <xsl:template match="SSL">
    <tr>
      <td><xsl:choose>
        <xsl:when test="@enable='true'">する</xsl:when>
        <xsl:when test="@enable='false'">しない</xsl:when>
      </xsl:choose></td>
      <td><xsl:choose>
        <xsl:when test="CFG_UNET3_SSL_VERSION=0">SSL v3.0</xsl:when>
        <xsl:when test="CFG_UNET3_SSL_VERSION=1">TLS v1.0</xsl:when>
        <xsl:when test="CFG_UNET3_SSL_VERSION=2">TLS v1.1</xsl:when>
      </xsl:choose></td>
      <td><xsl:value-of select="CFG_UNET3_SSL_CERT_SIZE"/></td>
      <td><xsl:value-of select="CFG_UNET3_SSL_CERT_DEPTH"/></td>
      <td><xsl:value-of select="CFG_UNET3_SSL_CA_CERT_MAX"/></td>
      <td><xsl:value-of select="CFG_UNET3_SSL_MAX_SESSION"/></td>
      <td><xsl:value-of select="CFG_UNET3_SSL_MAX_CONNECTION"/></td>
      <td><xsl:value-of select="CFG_UNET3_SSL_NET_BUF_MPF_SZ"/></td>
      <td><xsl:value-of select="CFG_UNET3_SSL_NET_BUF_MPF_BLK"/></td>
      <td><xsl:choose>
        <xsl:when test="CFG_UNET3_SSL_CIPHER_SPEC0='true'">有効</xsl:when>
        <xsl:when test="CFG_UNET3_SSL_CIPHER_SPEC0='false'">無効</xsl:when>
      </xsl:choose></td>
      <td><xsl:choose>
        <xsl:when test="CFG_UNET3_SSL_CIPHER_SPEC1='true'">有効</xsl:when>
        <xsl:when test="CFG_UNET3_SSL_CIPHER_SPEC1='false'">無効</xsl:when>
      </xsl:choose></td>
      <td><xsl:choose>
        <xsl:when test="CFG_UNET3_SSL_CIPHER_SPEC2='true'">有効</xsl:when>
        <xsl:when test="CFG_UNET3_SSL_CIPHER_SPEC2='false'">無効</xsl:when>
      </xsl:choose></td>
      <td><xsl:choose>
        <xsl:when test="CFG_UNET3_SSL_CIPHER_SPEC3='true'">有効</xsl:when>
        <xsl:when test="CFG_UNET3_SSL_CIPHER_SPEC3='false'">無効</xsl:when>
      </xsl:choose></td>
      <td><xsl:choose>
        <xsl:when test="CFG_UNET3_SSL_CIPHER_SPEC4='true'">有効</xsl:when>
        <xsl:when test="CFG_UNET3_SSL_CIPHER_SPEC4='false'">無効</xsl:when>
      </xsl:choose></td>
      <td><xsl:value-of select="CFG_UNET3_SSL_CRYP_TMO"/></td>
      <td><xsl:value-of select="CFG_UNET3_SSL_HASH_TMO"/></td>
      <td><xsl:choose>
        <xsl:when test="CFG_UNET3_SSL_ALLOW_MULTSK='true'">する</xsl:when>
        <xsl:when test="CFG_UNET3_SSL_ALLOW_MULTSK='false'">しない</xsl:when>
      </xsl:choose></td>


    </tr>
  </xsl:template>



  </xsl:stylesheet>
