
= Claude Agent SDKで作るAIエージェント開発入門

== はじめに

こんんちは、noguです。普段は、社内システムの開発、AIエージェントの開発、AI推進に携わっております。

業務でAIエージェントの開発を行っていますが、高性能なエージェントの開発を難易度が高いと感じています。

その中で開発に使用しているClaude Codeはエージェントとしてかなり性能が高いなと感じています。そこでこのClaude CodeがツールキットになっているClaude Agent SDKに目をつけました。

本章では、Claude Agent SDKの概要とハンズオン形式でAIエージェントの作成を紹介していきます。

== AIエージェント
#@# TODO:AIエージェント（エージェント）の定義について修正する

AIエージェントとは、LLMがさまざまざツールをを活用して複雑なタスクを自律的に遂行するシステムのことです。

従来のシステムでは難しかった変数の多い作業への対応が可能になります。

これにより、人と判断が必要であった作業への対応が可能になりました。

しかし、高性能なAIエージェントの構築は困難を極めます。

それに対する解決策として私が提案するのがClaude Agent SDKを用いてAIエージェントを作成することである。

== Claude Agent SDK

=== 概要
Anthropic社が提供する、AIエージェントを簡単に開発するための公式ツールキット（SDK）です。

同社のCLI型エージェントであるClaude Codeをライブラリとしていて、本格的なAIエージェントを従来より手間が少ない開発することできます。

=== Claude Code
ターミナルで実行されるえージェンティックアシスタントです。コーディングングに優れていますが、コマンドラインかrできることなら何でも手助けできます。ドキュメントの作成。ビルド実行、ファイル検索、トピック調査など、様々なタスクに対応することができます。

== 他のClaudeツールとの比較

=== Agent SDK vs Client SDK

Anthropic Client SDKは直接的なAPIアクセスのみを提供します。

そのため。プロンプトを送信、ツール実行は自分で実装する必要があります。

一方、Agent SDKは、組み込みのツール実行を備えたClaudeを提供します。

//table[agent-vs-client][Agent SDK vs Client SDK]{
項目	Client SDK	Agent SDK
------------
概要	直接的なAPIアクセスを提供	組み込みのツール実行を備えたClaudeを提供
ツールループ	自分で実装する必要がある	Claudeが自律的に処理する
ツール実行	開発者が自分で呼び出す	自動的に実行される
//}

=== Agent SDK vs Claude Code CLI

Agent SDKとClaude Codeは同じ機能を、異なるインターフェースで提供しています。

アプリケーションの開発自体はClaude Codeが向いていますが、AIを組み込んだアプリケーションはClaude Agent SDKが向いています。


//table[agent-vs-cli-usecase][ツール別の向いている作業]{
Claude Agent SDK	Claude Code
------------
CI/CDパイプライン	インタラクティブな開発
カスタムアプリケーション	単発のタスク
本番自動化	.
//} 

== 機能
=== 組み込みツール
エージェントが使用できるツールがいくつか組み込まれています。これにより、ファイルの読み込みやコマンド実行、検索などをすぎに実行することができます。

//table[including-tool][組み込みツール]{
名前	機能
------------
読む	作業ディレクトリ内の任意のファイルを読み取る
書く	新しいファイルを作成する
編集	既存のファイルを正確に編集する
バッシュ	ターミナルコマンド、スクリプト、git操作を実行する
グロブ	パターン（**/*.ts、src/**/*.py）でファイルを検索する
グレップ	正規表現でファイルの内容を検索する
ウェブ検索	最新情報をウェブで検索する
ウェブフェッチ	ウェブページのコンテンツを取得して解析する
ユーザーに質問する	複数の選択肢からユーザーに明確な質問をする
//}

=== フック
エージェント動作の重要なポイントでカスタムコードを実行します。エージェント動作の検証、ログ記録、作業のブロック、また変換を実行します。

https://platform.claude.com/docs/en/agent-sdk/hooks

=== サブエージェント
特定のサブタスクを処理するための専門エージェント機能。メインのエージェントが作業を委任し、サブエージェントが結果のみを報告します。

https://platform.claude.com/docs/en/agent-sdk/subagents

=== MCP

モデルコンテキストプロトコル（以下。MCP）を介して、デーバベース、ブラウザ、APIなどの外部システムに接続することができます。

これにより、AIエージェント内に存在しないような最新情報や社内独自のデータベースにもアクセスすることが可能になります。

=== 権限

エージェントが使用できるツールを正確に制御することができます。

これにより、安全な操作を許可し、危険な操作のみブロックし、機密性の高い操作には承認を求めることができます。

=== セッション
エージェントとの複数回のやりとりに渡って、会話の内容（コンテキスト）を維持します。エージェントは読み取ったファイル、実行した分析、会話履歴を記憶します。

これにより、後で会話（セッション）を再開すること、会話のコピー（フォーク）をして異なるアプローチで実行をすることができます。

== TypeScript SDK、Python SDK
Claude Agent SDKは、TypeScriptとPythonので提供されています。本章ではTypeScript SDKを採用しています。

== クイックスタート

Claude Agent SDKを使って、最小限のAIエージェントを構築してみましょう。このセクションでは、ユーザーのプロンプトに対してClaudeが自律的に対応するシンプルなエージェントを作成します。

=== 前提条件

* Node.js 18+
* Anthropic アカウント（https://platform.claude.com/ でサインアップ）
* 有効なAPI キー

=== セットアップ

==== プロジェクトフォルダの作成

//list[project-setup][プロジェクトフォルダの作成]{
mkdir claude-agent-sdk-demo && cd claude-agent-sdk-demo
//}

==== SDK のインストール

//list[install-sdk][SDK のインストール]{
npm install @anthropic-ai/claude-agent-sdk
//}

==== API キーの設定

プロジェクトディレクトリに `.env` ファイルを作成し、API キーを設定します：

//list[env-setup][API キーの設定]{
ANTHROPIC_API_KEY='<あなたのAPIキー>'
//}

=== 初めてのエージェントを作成する

`quickstart.ts` を作成し、以下のコードを実装します：

//listnum[quickstart-code][初めてのエージェント]{
import Anthropic from "@anthropic-ai/sdk";

async function main() {
  const anthropic = new Anthropic();

  const msg = await anthropic.messages.create({
    model: "claude-opus-4-6",
    max_tokens: 1000,
    messages: [
      {
        role: "user",
        content: "再生可能エネルギー分野の最新動向を調べるには、何を検索すればよいでしょうか？"
      }
    ]
  });
  console.log(msg);
}

main().catch(console.error);
//}

=== エージェントを実行する

//list[run-agent][エージェントを実行する]{
npx tsx quickstart.ts
//}

実行後、プロンプトを入力すると、Claude がリアルタイムで応答を生成します。

=== 出力例

//listnum[output-example][出力例]{
{
  model: 'claude-opus-4-6',
  id: 'msg_01WqcJjSx9EZJFUyERMvqaVV',
  type: 'message',
  role: 'assistant',
  content: [
    {
      type: 'text',
      text: '# 再生可能エネルギー分野の最新動向を調べるための検索ガイド\n' +
        '\n' +
        '## 🔍 おすすめの検索キーワード\n' +
        '\n' +
        '### 全般的な動向\n' +
        '- 「再生可能エネルギー 最新動向 2024」\n' +
        '- 「renewable energy trends 2024」\n' +
        '- 「クリーンエネルギー 技術革新」\n' +
        '- 「再エネ 政策動向」\n' +
        '\n' +
        '### 分野別の検索キーワード\n' +
        '\n' +
        '| 分野 | 日本語キーワード | 英語キーワード |\n' +
        '|------|----------------|---------------|\n' +
        '| 太陽光 | ペロブスカイト太陽電池 最新 | perovskite solar cell |\n' +
        '| 洋上風力 | 浮体式洋上風力 日本 | floating offshore wind |\n' +
        '| 水素 | グリーン水素 技術開発 | green hydrogen |\n' +
        '| 蓄電池 | 全固体電池 / 定置型蓄電池 | solid-state battery |\n' +
        '| 次世代 | 核融合 進捗 / 波力発電 | fusion energy / wave energy |\n' +
        '\n' +
        '---\n' +
        '\n' +
        '## 📚 信頼性の高い情報源\n' +
        '\n' +
        '### 日本語\n' +
        '- **資源エネルギー庁**（経済産業省）のウェブサイト\n' +
        '- **NEDO**（新エネルギー・産業技術総合開発機構）\n' +
        '- **IRENA**（国際再生可能エネルギー機関）日本語資料\n' +
        '- **環境ビジネスオンライン** / **スマートジャパン**\n' +
        '\n' +
        '### 英語（最先端の情報）\n' +
        '- **IEA**（国際エネルギー機関）のレポート\n' +
        '- **BloombergNEF**（BNEF）\n' +
        '- **IRENA** の年次報告書\n' +
        '- **Nature Energy** / **Joule**（学術誌）\n' +
        '- **PV Magazine** / **Recharge News**\n' +
        '\n' +
        '---\n' +
        '\n' +
        '## 🎯 現在注目されているトピック例\n' +
        '\n' +
        '1. **ペロブスカイト太陽電池**の商用化動向\n' +
        '2. **浮体式洋上風力発電**の大規模展開\n' +
        '3. **グリーン水素**のコスト低減と供給網構築\n' +
        '4. **系統用蓄電池**と電力グリッドの安定化\n' +
        '5. **GX（グリーントランスフォーメーション）** 政策\n' +
        '6. **PPA（電力購入契約）** モデルの普及\n' +
        '7. **カーボンクレジット**と再エネの関係\n' +
        '\n' +
        '---\n' +
        '\n' +
        '## 💡 効率的な調べ方のコツ\n' +
        '\n' +
        '- **Google Scholar** で学術論文を検索する\n' +
        '- **「filetype:pdf」** を加えると報告書が見つかりやすい\n' +
        '- 英語で検索すると情報量が格段に増える\n' +
        '- SNS（X/LinkedIn）で業界専門家をフォローする\n' +
        '\n' +
        '特定の分野やテーマについてさらに詳しく知りたい場合は、お気軽にお聞きください！'
    }
  ],
  stop_reason: 'end_turn',
  stop_sequence: null,
  usage: {
    input_tokens: 44,
    cache_creation_input_tokens: 0,
    cache_read_input_tokens: 0,
    cache_creation: { ephemeral_5m_input_tokens: 0, ephemeral_1h_input_tokens: 0 },
    output_tokens: 860,
    service_tier: 'standard',
    inference_geo: 'global'
  }
}
//}

=== 主要な概念

#@# TODO:もっとちゃんと書いてください

**query 関数**はエージェントループの中核です。非同期イテレータを返すため、Claudeが思考・推論する間、メッセージをストリーミングして表示できます。

**prompt**はClaudeに実行してほしいタスクを指定します。ここではツールを使わないため、Claudeの自然言語による回答がそのまま結果になります。

**options**でエージェントの動作をカスタマイズできます。デフォルトではツール機能が有効になりますが、この例では基本的な対話を行っています。

== AIエージェントを作ってみる①

== AIエージェントを作ってみる②

== AIエージェントを作ってみる③

== Claude Agent SDKが使われている事例

=== Xcode

2026年2月に追加されました
https://www.anthropic.com/news/apple-xcode-claude-agent-sdk

=== Craft Agents

https://agents.craft.do/


== おわりに

== 参考文献

• Building effective agents：https://www.anthropic.com/en/gineering/building-effective-agents
• Agent SDK 概要（日本語）：https://platform.claude.com/docs/ja/agent-sdk/overview
• Agent SDK 概要（英語版）：https://platform.claude.com/docs/en/agent-sdk/overview
• Claude Code SDK Demo：https://github.com/anthropics/claude-agent-sdk-demos?tab=readme-ov-file
• Apple's Xcode now supports the Claude Agent SDK：https://www.anthropic.com/news/apple-xcode-claude-agent-sdk
• Claude Agent SDK [Full Workshop] — Thariq Shihipar, Anthropic：https://www.youtube.com/watch?v=TqC1qOfiVcQ&t=5s