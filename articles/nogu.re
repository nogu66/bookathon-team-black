
= Claude Agent SDKで作るAIエージェント開発入門

== はじめに

こんんちは、noguです。普段は、社内システムの開発、AIエージェントの開発、AI推進に携わっております。最近、業務でAIエージェントの開発に取り組んでいますが、高性能なエージェントの開発を難易度が高いと感じています。エージェントの本質はシステムのループが本質と言われることがあります。特にループの制御が難しいにも関わらず、ツールの呼び出し、生成AIへの指示なども必要となります。

AIエージェントという観点では、開発に使用しているClaude Codeがかなり性能が高いなと感じています。そこでClaude CodeをツールキットとしているClaude Agent SDKに目をつけました。これにより、

本章では、Claude Agent SDKの仕組みとClaude Agent SDKで作成されたエージェントについて紹介していきます。

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

== 主要な概念

Claude Agent SDKを使いこなすために、3つの主要な概念を理解しておきましょう。

=== query関数

query関数は、エージェントループの中核となるエントリーポイントです。この関数がClaudeとのやりとり全体を管理します。

query関数は非同期イテレータ（AsyncGenerator）を返します。これにより、Claudeがツールを実行しながら思考・推論する過程をリアルタイムにストリーミングで受け取ることができます。

//listnum[query-basic][query関数の基本的な使い方]{
import { query } from "@anthropic-ai/claude-agent-sdk";

const q = query({
  prompt: "srcディレクトリのバグを見つけて修正してください",
  options: { model: "claude-sonnet-4-5-20250929" }
});

for await (const message of q) {
  if (message.type === "result") {
    console.log(message.result);
  }
}
//}

エージェントループとは、Claudeがタスクを完了するまで「思考→ツール実行→結果の確認」を繰り返す仕組みです。開発者がループを自分で実装する必要はありません。query関数がこのループ全体を自動的に制御します。

=== prompt

promptは、Claudeに実行してほしいタスクを自然言語で指定します。

従来のAPIでは「どのツールをいつ呼ぶか」を開発者が制御する必要がありましたが、Agent SDKではpromptにゴールを記述するだけで、Claudeが必要なツールを自律的に選択して実行します。

たとえば「srcディレクトリのバグを見つけて修正してください」というpromptを与えると、Claudeはファイルの読み込み、コードの検索、問題の特定、ファイルの編集といった一連の作業を自動で行います。

=== options

optionsは、エージェントの動作を細かくカスタマイズするための設定オブジェクトです。主要な設定項目は以下のとおりです。

//table[options-table][optionsの主要な設定項目]{
項目	説明
------------
model	使用するClaudeモデルを指定する
allowedTools	エージェントが使用できるツールを制限する
permissionMode	ファイル編集やコマンド実行の承認方法を制御する
systemPrompt	エージェントの振る舞いを指示するシステムプロンプトを設定する
maxTurns	エージェントループの最大ターン数を制限する
mcpServers	外部システムに接続するMCPサーバーを設定する
//}

たとえば、バグ修正エージェントではファイルの読み書きだけを許可し、コマンド実行は禁止する、といった制御が可能です。

//listnum[options-example][optionsの設定例]{
const q = query({
  prompt: "srcディレクトリのバグを見つけて修正してください",
  options: {
    model: "claude-sonnet-4-5-20250929",
    allowedTools: ["Read", "Edit", "Glob", "Grep"],
    permissionMode: "acceptEdits",
    maxTurns: 10
  }
});
//}

この例では、Claudeにファイルの読み取り・編集・検索のみを許可し、ファイル変更は自動承認、最大10ターンで処理を完了するよう設定しています。

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
        （略）
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


== Claude Agent SDKデモリポジトリ

クイックスタートで基本的な使い方を確認したところで、より実践的なアプリケーションを見ていきましょう。

Anthropic社が公式で提供しているデモリポジトリ（@<href>{https://github.com/anthropics/claude-agent-sdk-demos}）には、Agent SDKの機能を活かした7つのデモプロジェクトが収録されています。

本節では、その中から特徴的な3つのデモを取り上げ、それぞれがどのようなSDK機能を活用しているかを紹介します。

=== シンプルチャットアプリ（simple-chatapp）

Webブラウザ上でClaudeと対話できるチャットアプリケーションです。ReactによるフロントエンドとExpressによるバックエンドで構成され、WebSocketを使ったリアルタイム通信を実現しています。

このデモは、Agent SDKをWebアプリケーションに組み込むための基本的なアーキテクチャパターンを示しています。

//table[simple-chatapp-architecture][シンプルチャットアプリのアーキテクチャ]{
レイヤー	技術	役割
------------
フロントエンド	React + Vite	チャットUI、WebSocket通信
バックエンド	Express + WebSocket	APIサーバー、エージェント管理
エージェント	Claude Agent SDK	query関数によるClaude呼び出し
//}

バックエンド側では、query関数をWebSocketと橋渡しするMessageQueueクラスを実装しています。これにより、ユーザーのメッセージをAgent SDKに渡し、Claudeの応答をリアルタイムでフロントエンドに配信します。

//listnum[chatapp-session][AgentSessionクラスの概要（簡略版）]{
import { query } from "@anthropic-ai/claude-agent-sdk";

class AgentSession {
  async processMessage(userMessage: string) {
    const q = query({
      prompt: userMessage,
      options: {
        model: "claude-sonnet-4-5-20250929",
        allowedTools: [
          "Bash", "Read", "Write", "Edit",
          "Glob", "Grep", "WebSearch", "WebFetch"
        ]
      }
    });

    for await (const message of q) {
      // WebSocket経由でフロントエンドに送信
      this.sendToClient(message);
    }
  }
}
//}

===={chatapp-sdk-features} 使用しているSDK機能

 * @<b>{query関数}：エージェントループの制御。非同期イテレータでメッセージをストリーミング受信
 * @<b>{組み込みツール}：Bash、Read、Write、Edit、Glob、Grep、WebSearch、WebFetchの8種類を許可
 * @<b>{allowedTools}：エージェントが使用できるツールをホワイトリストで制限

=== リサーチエージェント（research-agent）

複数のサブエージェントが協調して調査レポートを作成するマルチエージェントシステムです。Python SDKで実装されています。

ユーザーが「量子コンピューティングについて調べて」のようなプロンプトを与えると、リードエージェントがタスクを分解し、専門のサブエージェントに作業を委任します。

//table[research-agent-roles][リサーチエージェントの役割分担]{
エージェント	役割	使用ツール
------------
リードエージェント	タスク分解と全体指揮	Task（サブエージェント起動）
リサーチャー	Webで情報を収集	WebSearch、Write
データアナリスト	データ整理と可視化	Glob、Read、Bash、Write
レポートライター	最終レポートの作成	Read、Write、Glob、Bash
//}

このデモの特徴は、サブエージェント機能とフック機能を組み合わせた高度なエージェント追跡システムです。

//listnum[research-agent-tracking][フックによるサブエージェント追跡（概要）]{
# PreToolUseフック：ツール実行前に記録
def pre_tool_hook(tool_name, tool_input, agent_id):
    tracker.record({
        "agent": agent_id,
        "tool": tool_name,
        "input": tool_input,
        "timestamp": datetime.now()
    })

# PostToolUseフック：ツール実行結果を記録
def post_tool_hook(tool_name, tool_output, agent_id):
    tracker.update_result(agent_id, tool_output)
//}

すべてのツール呼び出しがフックで記録され、どのエージェントがいつ何を実行したかをトランスクリプトとして出力します。

===={research-sdk-features} 使用しているSDK機能

 * @<b>{サブエージェント}：Taskツールによる専門エージェントの起動と並列実行
 * @<b>{フック}：PreToolUse・PostToolUseフックでツール呼び出しを監視・記録
 * @<b>{組み込みツール}：エージェントごとに異なるツールセットを割り当て
 * @<b>{権限}：bypassPermissionsモードによる自動実行

=== レジュメジェネレーター（resume-generator）

人物名を入力すると、Webで情報を検索し、プロフェッショナルな履歴書をWord文書（.docx）として自動生成するエージェントです。

//listnum[resume-usage][レジュメジェネレーターの実行]{
npm start "Dario Amodei"
//}

このコマンドを実行すると、Claudeは以下のステップを自律的に実行します。

 1. WebSearchで対象人物のLinkedIn、GitHub、企業ページ、ニュース記事などを検索
 2. 収集した情報から職歴、学歴、スキルを抽出・整理
 3. docxライブラリを使用した履歴書生成スクリプトをJavaScriptで作成
 4. Bashツールでスクリプトを実行し、フォーマット済みの.docxファイルを出力

//listnum[resume-query][レジュメジェネレーターの実装（簡略版）]{
const q = query({
  prompt: `${personName}の履歴書を作成してください`,
  options: {
    model: "claude-sonnet-4-5-20250929",
    allowedTools: [
      "WebSearch", "WebFetch", "Bash",
      "Write", "Read", "Glob"
    ],
    cwd: "./agent",
    settingSources: ["project"]
  }
});
//}

===={resume-sdk-features} 使用しているSDK機能

 * @<b>{WebSearch・WebFetch}：Web検索による情報収集
 * @<b>{スキル}：settingSourcesで.claude/skills/ディレクトリからカスタムスキルを読み込み、ドキュメント生成の手順をエージェントに教示
 * @<b>{Bashツール}：生成したスクリプトの実行
 * @<b>{作業ディレクトリ}：cwdオプションでエージェントの作業範囲を制限

=== デモリポジトリのまとめ

3つのデモを通じて、Claude Agent SDKの機能がどのように実践的なアプリケーションで活用されるかを確認しました。

//table[demo-summary][デモで使用されたSDK機能のまとめ]{
機能	チャットアプリ	リサーチエージェント	レジュメジェネレーター
------------
query関数	○	○	○
組み込みツール	○	○	○
サブエージェント	.	○	.
フック	.	○	.
スキル	.	.	○
権限制御	○	○	○
//}

このように、同じSDKの機能を組み合わせることで、Webアプリケーション、マルチエージェントシステム、ドキュメント生成ツールといった多様なAIエージェントを構築できます。

== Claude Agent SDKが使われている事例

=== Xcode

//image[images/nogu/xcode][XcodeとClaude Agent SDKの統合]{
//}

2026年2月、AppleのXcode 26.3にClaude Agent SDKがネイティブ統合されました。これにより、開発者はIDE内で直接エージェンティックなコーディング支援を利用できるようになりました。

主な機能は以下の4つです。

 * ビジュアル検証：Claudeがリアルタイムでプレビューを表示し、UIの問題を特定・修正できる。SwiftUI開発で特に有効
 * プロジェクト全体の横断的な推論：個別のファイルだけでなく、プロジェクト全体の構造を理解し、アーキテクチャ上の判断を下すことができる
 * 自律的なタスク実行：具体的な指示ではなくゴールを伝えるだけで、Claudeがタスクを分解し、必要なファイルの変更やAppleのドキュメント検索を自動で行う
 * MCPインターフェース：Claude Code CLIからMCP経由でXcodeに接続し、コマンドラインからビジュアルプレビューにアクセスすることも可能

=== Craft Agents

Craft Agentsは、Claude Agent SDKを基盤としたオープンソースのデスクトップアプリケーションです。Apache 2.0ライセンスで公開されています。

//image[images/nogu/craft-agent][Craft Agent]{
//}

Claude Codeと同様のエージェント機能を、デスクトップアプリのUIで提供しています。ドキュメント中心のワークフローに最適化されたインターフェースが特徴です。そのため。CLIベースのツールに慣れないユーザーにとっては適したアプリといえます。

主な機能は以下のとおりです。

 * ソース連携：MCP サーバー、REST API（Google、Slack、Microsoftなど）、ローカルファイルシステムなど、32以上のデータソースに接続可能
 * セッション管理：Todo → In Progress → Needs Review → Done のステータスワークフローで作業を管理
 * 権限モード：Explore（読み取り専用）、Ask to Edit（承認が必要）、Auto（自動実行）の3段階で、エージェントの自律性を制御

== おわりに

== 参考文献

 * Building effective agents：https://www.anthropic.com/en/gineering/building-effective-agents
 * Agent SDK 概要（日本語）：https://platform.claude.com/docs/ja/agent-sdk/overview
 * Agent SDK 概要（英語版）：https://platform.claude.com/docs/en/agent-sdk/overview
 * Claude Code SDK Demo：https://github.com/anthropics/claude-agent-sdk-demos?tab=readme-ov-file
 * Apple's Xcode now supports the Claude Agent SDK：https://www.anthropic.com/news/apple-xcode-claude-agent-sdk
 * Claude Agent SDK [Full Workshop] — Thariq Shihipar, Anthropic：https://www.youtube.com/watch?v=TqC1qOfiVcQ&t=5s