<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

  <xsl:template match="Kernel">
    <p><hr/></p>
    <h3>[カーネルの設定値]</h3>

    <h4>カーネル全般</h4>
    <xsl:apply-templates select="General" />

    <h4>タスク</h4>
    <table>
    <tr>
    <th>IDの定義名</th>
    <th>関数名</th>
    <th>優先度の初期値</th>
    <th>拡張情報</th>
    <th>(ローカル)スタックサイズ</th>
    <th>タスク属性</th>
    <th>共有スタック</th>
    </tr>
    <xsl:apply-templates select="Task" />
    </table>

    <h4>セマフォ</h4>
    <table>
    <tr>
    <th>IDの定義名</th>
    <th>資源数の初期値</th>
    <th>最大資源数</th>
    <th>セマフォ属性</th>
    </tr>
    <xsl:apply-templates select="Semaphore" />
    </table>

    <h4>イベントフラグ</h4>
    <table>
    <tr>
    <th>IDの定義名</th>
    <th>ビットパターン初期値(HEX)</th>
    <th>イベントフラグ属性</th>
    </tr>
    <xsl:apply-templates select="EventFlag" />
    </table>

    <h4>データキュー</h4>
    <table>
    <tr>
    <th>IDの定義名</th>
    <th>データの個数</th>
    <th>データキュー属性</th>
    </tr>
    <xsl:apply-templates select="DataQueue" />
    </table>

    <h4>メールボックス</h4>
    <table>
    <tr>
    <th>IDの定義名</th>
    <th>メールボックス属性</th>
    </tr>
    <xsl:apply-templates select="Mailbox" />
    </table>

    <h4>固定長メモリプール</h4>
    <table>
    <tr>
    <th>IDの定義名</th>
    <th>メモリブロック数</th>
    <th>メモリブロックのサイズ</th>
    <th>固定長メモリプール属性</th>
    </tr>
    <xsl:apply-templates select="FixedMemPool" />
    </table>

    <h4>周期ハンドラ</h4>
    <table>
    <tr>
    <th>IDの定義名</th>
    <th>関数名</th>
    <th>拡張情報</th>
    <th>起動周期</th>
    <th>起動位相</th>
    <th>周期ハンドラ属性</th>
    </tr>
    <xsl:apply-templates select="CyclicHandler" />
    </table>

    <h4>割込みサービスルーチン</h4>
    <table>
    <tr>
    <th>割込み番号</th>
    <th>関数名</th>
    <th>拡張情報</th>
    </tr>
    <xsl:apply-templates select="Isr" />
    </table>

    <h4>カーネル管理外割込み</h4>
    <table>
    <tr>
    <th>割込み番号</th>
    <th>関数名</th>
    </tr>
    <xsl:apply-templates select="NonKnlIsr" />
    </table>

    <h4>共有スタック</h4>
    <table>
    <tr>
    <th>IDの定義名</th>
    <th>スタックのサイズ</th>
    </tr>
    <xsl:apply-templates select="SharedStack" />
    </table>
  </xsl:template>

  <xsl:template match="Kernel/General">
    <table>
      <tr>
        <th>カーネル割込みレベル</th>
        <th>タスク優先度</th>
        <th>チック時間</th>
        <th>初期化関数</th>
        <th>アイドル関数</th>
        <th>追加ヘッダファイル</th>
        <th>タイムイベントハンドラスタックサイズ(CSTACK)</th>
        <th>システムハンドラスタックサイズ(HSTACK)</th>
      </tr>
      <tr>
        <td><xsl:value-of select="CFG_KNLMSKLVL"/></td>
        <td><xsl:value-of select="CFG_MAXTSKPRI"/></td>
        <td><xsl:value-of select="CFG_TICK"/></td>
        <td><xsl:value-of select="CFG_INITFUNC"/></td>
        <td><xsl:value-of select="CFG_IDLEFUNC"/></td>
        <td><xsl:value-of select="CFG_ADDHEADER"/></td>
        <td><xsl:value-of select="CFG_CSTKSZ"/></td>
        <td><xsl:value-of select="CFG_HSTKSZ"/></td>
      </tr>
    </table>
  </xsl:template>

  <xsl:template match="Task">
    <tr>
      <td><xsl:value-of select="@CFG_TSKID"/></td>
      <td><xsl:value-of select="CFG_TASK"/></td>
      <td><xsl:value-of select="CFG_ITSKPRI"/></td>
      <td><xsl:value-of select="CFG_TSKEXINF"/></td>
      <td><xsl:value-of select="CFG_STKSZ"/></td>
      <td><xsl:value-of select="CFG_TSKATR"/></td>
      <td><xsl:value-of select="CFG_SSTKID"/></td>
    </tr>
  </xsl:template>
  
  <xsl:template match="Semaphore">
    <tr>
      <td><xsl:value-of select="@CFG_SEMID"/></td>
      <td><xsl:value-of select="CFG_ISEMCNT"/></td>
      <td><xsl:value-of select="CFG_MAXSEM"/></td>
      <td><xsl:value-of select="CFG_SEMATR"/></td>
    </tr>
  </xsl:template>

  <xsl:template match="EventFlag">
    <tr>
      <td><xsl:value-of select="@CFG_FLGID"/></td>
      <td><xsl:value-of select="CFG_IFLGPTN"/></td>
      <td><xsl:value-of select="CFG_FLGATR"/></td>
    </tr>
  </xsl:template>

  <xsl:template match="DataQueue">
    <tr>
      <td><xsl:value-of select="@CFG_DTQID"/></td>
      <td><xsl:value-of select="CFG_DTQCNT"/></td>
      <td><xsl:value-of select="CFG_DTQATR"/></td>
    </tr>
  </xsl:template>

  <xsl:template match="Mailbox">
    <tr>
      <td><xsl:value-of select="@CFG_MBXID"/></td>
      <td><xsl:value-of select="CFG_MBXATR"/></td>
    </tr>
  </xsl:template>

  <xsl:template match="FixedMemPool">
    <tr>
      <td><xsl:value-of select="@CFG_MPFID"/></td>
      <td><xsl:value-of select="CFG_BLKCNT"/></td>
      <xsl:choose>
        <xsl:when test="CFG_MPFDIRECT=1">
          <td><xsl:value-of select="CFG_BLKSZ"/></td>
        </xsl:when>
        <xsl:when test="CFG_MPFDIRECT='true'">
          <td><xsl:value-of select="CFG_BLKSZ"/></td>
        </xsl:when>
        <xsl:when test="CFG_MPFDIRECT=0">
          <td><xsl:value-of select="CFG_MPFEXP"/></td>
        </xsl:when>
        <xsl:when test="CFG_MPFDIRECT='false'">
          <td><xsl:value-of select="CFG_MPFEXP"/></td>
        </xsl:when>
      </xsl:choose>
      <td><xsl:value-of select="CFG_MPFATR"/></td>
    </tr>
  </xsl:template>

  <xsl:template match="CyclicHandler">
    <tr>
      <td><xsl:value-of select="@CFG_CYCID"/></td>
      <td><xsl:value-of select="CFG_CYCHDR"/></td>
      <td><xsl:value-of select="CFG_CYCEXINF"/></td>
      <td><xsl:value-of select="CFG_CYCTIM"/></td>
      <td><xsl:value-of select="CFG_CYCPHS"/></td>
      <td><xsl:value-of select="CFG_CYCATR"/></td>
    </tr>
  </xsl:template>
  
  <xsl:template match="Isr">
    <tr>
      <td><xsl:value-of select="CFG_ISRINTNO"/></td>
      <td><xsl:value-of select="CFG_ISR"/></td>
      <td><xsl:value-of select="CFG_ISREXINF"/></td>
    </tr>
  </xsl:template>

  <xsl:template match="NonKnlIsr">
    <tr>
      <td><xsl:value-of select="CFG_NKISRINTNO"/></td>
      <td><xsl:value-of select="CFG_NKISR"/></td>
    </tr>
  </xsl:template>

  <xsl:template match="SharedStack">
    <tr>
      <td><xsl:value-of select="@CFG_SSTKID"/></td>
      <td><xsl:value-of select="CFG_SSTKSZ"/></td>
    </tr>
  </xsl:template>


</xsl:stylesheet>
