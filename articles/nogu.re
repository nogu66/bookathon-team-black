
= Claude Agent SDKで作るAIエージェント開発入門

== はじめに

こんにちは、noguです。普段は、社内システムの開発、AIエージェントの開発、AI推進に携わっております。最近、業務でAIエージェントの開発に取り組んでいますが、高性能なエージェントの開発は難易度が高いと感じています。エージェントの本質はループにあると言われることがあります。特にループの制御が難しいにも関わらず、ツールの呼び出し、生成AIへの指示なども必要となります。

AIエージェントという観点では、開発に使用しているClaude Codeがかなり性能が高いなと感じています。そこでClaude CodeをツールキットとしているClaude Agent SDKに目をつけました。

本章では、Claude Agent SDKの仕組みとClaude Agent SDKで作成されたエージェントについて紹介していきます。

== AIエージェント

まず、AIエージェントとは、LLMがさまざまなツールを活用して複雑なタスクを自律的に遂行するシステムのことです。具体的には、ユーザーからの指示を受け取り、必要な情報の検索、ファイルの読み書き、コマンドの実行といったツールを組み合わせながら、目標達成まで「思考→ツール実行→結果の確認」というループを自律的に回し続けます。単にLLMがテキストを生成するだけではなく、外部のツールと連携しながら実際のタスクを完遂できる点が特徴です。

従来のシステムでは、あらかじめ決められたルールやフローに沿って処理を行うため、状況に応じた柔軟な判断が求められる作業への対応が困難でした。たとえば、コードレビューではファイルの読み込み、関連コードの検索、修正案の作成といった複数のステップが必要ですが、従来のシステムではこれらの手順をすべて事前にルール化しなければなりませんでした。AIエージェントはLLMの推論能力により、状況に応じて次に取るべきアクションを柔軟に判断しながらタスクを実行できます。これにより、従来は人の判断が必要であったコードの品質評価やドキュメントの要約といった作業にも対応できるようになりました。

しかし、高性能なAIエージェントの構築は困難を極めます。ツールの呼び出しタイミングの制御、エラー時のリトライ戦略、会話コンテキストの管理、そしてエージェントループの実装など、考慮すべき要素が多岐にわたります。特にエージェントの本質であるループの制御は難易度が高く、いつループを継続し、いつ終了するかの判断を適切に行う必要があります。

この課題に対する解決策として、本章ではClaude Agent SDKを用いたAIエージェントの開発を提案します。Claude Agent SDKは、これらの複雑な制御をSDK側で抽象化し、開発者はゴールの指定とツールの選定に集中できるようにしたツールキットです。次節以降で、その仕組みと具体的な使い方を見ていきましょう。

== Claude Agent SDK

=== 概要
Claude Agent SDKとは、Anthropic社が提供する「AIエージェントを簡単に開発するための公式ツールキット（SDK）」です。同社のCLI型エージェントであるClaude Codeをライブラリとして利用しており、Claude Codeが持つエージェントループ、ツール実行、コンテキスト管理といった機能をプログラムから呼び出すことができます。

前節で述べたように、AIエージェントの構築にはループ制御やツール管理など多くの課題がありますが、Claude Agent SDKはこれらをすでに解決済みの部品として提供します。開発者はエージェントループを自分で実装する必要がなく、プロンプトとオプションを指定するだけで、Claudeが自律的にタスクを遂行するエージェントを構築できます。TypeScriptとPythonの両方で提供されており、既存のWebアプリケーションやCI/CDパイプラインへの組み込みも容易です。

=== Claude Code
Claude Codeとは、同様にAnthropic社が提供するターミナルで実行されるエージェンティックアシスタントです。コーディングに優れていますが、コマンドラインからできることなら何でも手助けできます。ドキュメントの作成、ビルド実行、ファイル検索、トピック調査など、さまざまなタスクに対応することができます。

Claude Agent SDKの文脈でClaude Codeが重要なのは、SDKの基盤となっている点です。Claude Codeは日々多くの開発者に利用されており、そのエージェントとしての性能は実証済みです。ファイルの読み書き、コマンド実行、Web検索といった組み込みツールや、エージェントループの制御ロジックは、すべてClaude Codeで培われたものです。Claude Agent SDKを使うということは、このClaude Codeの実績あるエージェント機能を、自分のアプリケーションの中で活用するということを意味します。

== 他のClaudeツールとの比較

=== Agent SDK vs Client SDK

Anthropic Client SDKは、Claude APIへの直接的なアクセスを提供するライブラリです。メッセージの送信やレスポンスの受信といった基本的なAPI通信を行えますが、エージェントとしての振る舞いは開発者が自分で実装する必要があります。具体的には、ツールの定義、ツール呼び出しの検知、ツールの実行、結果をClaudeに返すループ処理など、エージェントループ全体を自前で構築しなければなりません。

一方、Agent SDKは、このエージェントループをSDK側で完全に管理します。開発者がプロンプトを渡すだけで、Claudeが必要なツールを自律的に選択・実行し、結果を確認しながらタスクを完了まで進めます。ツールの定義や実行基盤も組み込みで提供されるため、開発者はエージェントの「動かし方」ではなく「何をさせるか」に集中できます。

//table[agent-vs-client][Agent SDK vs Client SDK]{
項目	Client SDK	Agent SDK
------------
概要	直接的なAPIアクセスを提供	組み込みのツール実行を備えたClaudeを提供
ツールループ	自分で実装する必要がある	Claudeが自律的に処理する
ツール実行	開発者が自分で呼び出す	自動的に実行される
組み込みツール	なし	ファイル操作、検索、Web検索など多数
//}

たとえば、Client SDKでファイルを読み取って分析するエージェントを作る場合、ツールの定義、Claudeからのツール呼び出しリクエストの解析、ファイル読み取りの実行、結果の返却、そして次のターンへの継続といった処理をすべて自分で書く必要があります。Agent SDKでは、これらが関数の1回の呼び出しに集約されます。

=== Agent SDK vs Claude Code CLI

Agent SDKとClaude Code CLIは、同じClaude Codeのエージェント機能を異なるインターフェースで提供しています。Claude Code CLIはターミナル上で対話的に使うツールであり、開発者自身がリアルタイムでClaudeとやりとりしながら作業を進めます。一方、Agent SDKはプログラムから呼び出すライブラリであり、アプリケーションの一部としてエージェント機能を組み込むことができます。

そのため、開発者が自分の作業を効率化したい場合はClaude Code CLIが適しており、エンドユーザー向けのアプリケーションにAI機能を組み込みたい場合やバックグラウンドで自動実行したい場合はAgent SDKが適しています。

//table[agent-vs-cli-usecase][ツール別の向いている作業]{
Claude Agent SDK	Claude Code CLI
------------
CI/CDパイプライン	インタラクティブな開発
カスタムアプリケーション	単発のタスク
本番自動化	探索的なデバッグ
//}

== 機能

Claude Agent SDKには、エージェントを構築するための機能が複数用意されています。ここでは主要な機能を紹介します。

=== 組み込みツール
エージェントが使用できるツールがあらかじめ組み込まれています。開発者がツールを自作しなくても、ファイルの読み書き、コマンド実行、Web検索などをすぐに利用できます。allowedToolsオプションで、エージェントに許可するツールをホワイトリスト形式で制限することも可能です。

//table[including-tool][組み込みツール]{
名前	機能
------------
Read	作業ディレクトリ内の任意のファイルを読み取る
Write	新しいファイルを作成する
Edit	既存のファイルを正確に編集する
Bash	ターミナルコマンド、スクリプト、git操作を実行する
Glob	パターン（**/*.ts、src/**/*.py）でファイルを検索する
Grep	正規表現でファイルの内容を検索する
WebSearch	最新情報をウェブで検索する
WebFetch	ウェブページのコンテンツを取得して解析する
AskUser	複数の選択肢からユーザーに明確な質問をする
//}

=== フック
フックは、エージェントの動作の重要なポイントでカスタムコードを実行する仕組みです。ツール実行の前後にフックを設定することで、エージェントの動作を検証したり、ログを記録したり、特定の操作をブロックしたりできます。

たとえば、PreToolUseフックでツール実行前に入力を検証し、不適切な操作を防止できます。PostToolUseフックではツール実行後の結果を記録し、エージェントの行動をトランスクリプトとして追跡できます。これにより、エージェントの動作を可視化し、デバッグや監査に活用することが可能です。

=== サブエージェント
サブエージェントは、特定のサブタスクを処理するための専門エージェント機能です。メインのエージェントが複雑なタスクを分解し、それぞれのサブタスクを専門のサブエージェントに委任します。サブエージェントは独立したコンテキストで動作し、完了後に結果のみをメインエージェントに報告します。

これにより、たとえば「調査」「データ分析」「レポート作成」といった異なる役割を持つエージェントを並列に実行し、効率的にタスクを処理できます。各サブエージェントには異なるツールセットや権限を割り当てることも可能です。

=== MCP
MCP（Model Context Protocol）は、エージェントを外部システムに接続するための標準プロトコルです。MCPサーバーを設定することで、データベース、ブラウザ、API、社内システムなどにエージェントからアクセスできるようになります。

これにより、エージェントの知識に含まれない最新情報や、社内独自のデータベース、外部サービスのAPIなどを活用したエージェントを構築できます。

=== 権限
権限機能により、エージェントが実行できる操作を細かく制御できます。安全な操作（ファイルの読み取りなど）は自動で許可し、危険な操作（ファイルの削除やコマンド実行など）はブロックまたはユーザーの承認を求める、といった設定が可能です。

permissionModeオプションで制御レベルを指定でき、たとえばacceptEditsモードではファイル編集を自動承認し、bypassPermissionsモードではすべての操作を承認なしで実行します。本番環境での自動実行と開発時の安全性を両立できる仕組みです。

=== セッション
セッション機能は、エージェントとの複数回のやりとりにわたって会話のコンテキストを維持します。エージェントが読み取ったファイル、実行した分析、会話の履歴がセッションに保存されます。

これにより、一度中断した作業を後から再開したり、セッションをフォーク（複製）して異なるアプローチを試したりすることができます。長時間にわたる調査や段階的なタスクにおいて、コンテキストを失うことなく作業を継続できる点が利点です。

== TypeScript SDK、Python SDK
Claude Agent SDKは、TypeScriptとPythonの2つの言語で提供されています。どちらも同じAgent SDKの機能を利用できますが、本章ではTypeScript SDKを採用しています。TypeScriptを選んだ理由は、Webアプリケーションとの親和性が高く、フロントエンドからバックエンドまで一貫した言語で開発できるためです。

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

`quickstart.ts` を作成し、以下のコードを実装します。Agent SDKのquery関数を使い、Claudeがツールを自律的に活用しながらタスクを遂行するエージェントを構築します。

//listnum[quickstart-code][初めてのエージェント]{
import { query } from "@anthropic-ai/claude-agent-sdk";

async function main() {
  const q = query({
    prompt: "現在のディレクトリにあるファイル一覧を取得して、その概要を教えてください",
    options: {
      model: "claude-sonnet-4-5-20250929",
      allowedTools: ["Read", "Glob", "Grep"],
      permissionMode: "acceptEdits",
      maxTurns: 5
    }
  });

  for await (const message of q) {
    if (message.type === "result") {
      console.log(message.result);
    }
  }
}

main().catch(console.error);
//}

このコードでは、Claudeにファイルの読み取りと検索のみを許可し、最大5ターンでタスクを完了するよう設定しています。Claudeは指示に基づいて、Globでファイルを検索し、Readで内容を読み取り、概要をまとめるという一連の作業を自律的に行います。

=== エージェントを実行する

//list[run-agent][エージェントを実行する]{
npx tsx quickstart.ts
//}

実行すると、Claudeがツールを使いながらタスクを処理し、結果がストリーミングで出力されます。


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
レポートライター	最終レポートの作成	Skill、Read、Write、Glob、Bash
//}

このデモの特徴は、サブエージェント機能とフック機能を組み合わせた高度なエージェント追跡システムです。

//listnum[research-agent-tracking][フックによるサブエージェント追跡（簡略版）]{
from claude_agent_sdk import HookMatcher
from claude_agent_sdk.types import HookInput, HookContext, HookJSONOutput

# PreToolUseフック：ツール実行前に記録
async def pre_tool_hook(
    hook_input: HookInput,
    tool_use_id: str | None,
    context: HookContext
) -> HookJSONOutput:
    tool_name = hook_input["tool_name"]
    tool_input = hook_input["tool_input"]
    tracker.record(tool_name, tool_input, tool_use_id)
    return {}

# PostToolUseフック：ツール実行結果を記録
async def post_tool_hook(
    hook_input: HookInput,
    tool_use_id: str | None,
    context: HookContext
) -> HookJSONOutput:
    tool_response = hook_input["tool_response"]
    tracker.update_result(tool_use_id, tool_response)
    return {}

# フックの登録
hooks = {
    "PreToolUse": [HookMatcher(matcher=None, hooks=[pre_tool_hook])],
    "PostToolUse": [HookMatcher(matcher=None, hooks=[post_tool_hook])],
}
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

//embed[latex]{
\begin{center}
\includegraphics[width=0.6\textwidth]{images/nogu/xcode.png}\\[2mm]
{\small ▲XcodeとClaude Agent SDKの統合}
\end{center}
//}

2026年2月、AppleのXcode 26.3にClaude Agent SDKがネイティブ統合されました。これにより、開発者はIDE内で直接エージェンティックなコーディング支援を利用できるようになりました。

主な機能は以下の4つです。

 * ビジュアル検証：Claudeがリアルタイムでプレビューを表示し、UIの問題を特定・修正できる。SwiftUI開発で特に有効
 * プロジェクト全体の横断的な推論：個別のファイルだけでなく、プロジェクト全体の構造を理解し、アーキテクチャ上の判断を下すことができる
 * 自律的なタスク実行：具体的な指示ではなくゴールを伝えるだけで、Claudeがタスクを分解し、必要なファイルの変更やAppleのドキュメント検索を自動で行う
 * MCPインターフェース：Claude Code CLIからMCP経由でXcodeに接続し、コマンドラインからビジュアルプレビューにアクセスすることも可能

=== Craft Agents

Craft Agentsは、Claude Agent SDKを基盤としたオープンソースのデスクトップアプリケーションです。Apache 2.0ライセンスで公開されています。

//embed[latex]{
\begin{center}
\includegraphics[width=0.6\textwidth]{images/nogu/craft-agent.png}\\[2mm]
{\small ▲Craft Agent}
\end{center}
//}

Claude Codeと同様のエージェント機能を、デスクトップアプリのUIで提供しています。ドキュメント中心のワークフローに最適化されたインターフェースが特徴です。そのため、CLIベースのツールに慣れないユーザーにとっては適したアプリといえます。

主な機能は以下のとおりです。

 * ソース連携：32以上のCraftドキュメントツールを搭載し、MCP サーバー、REST API（Google、Slack、Microsoftなど）、ローカルファイルシステムを通じて多様なデータソースに接続可能
 * セッション管理：Todo → In Progress → Needs Review → Done のステータスワークフローで作業を管理
 * 権限モード：Explore（読み取り専用）、Ask to Edit（承認が必要）、Auto（自動実行）の3段階で、エージェントの自律性を制御

== おわりに

本章では、Claude Agent SDKの仕組みと、それを活用したAIエージェントの構築方法を紹介しました。

AIエージェントの開発において最も難しいのは、エージェントループの制御やツール管理といった基盤部分の実装です。Claude Agent SDKは、Claude Codeで実証済みのこれらの機能をライブラリとして提供することで、開発者がエージェントの「何をさせるか」に集中できる環境を実現しています。

デモリポジトリや実際の事例で見たように、同じSDKの機能を組み合わせることで、Webアプリケーション、マルチエージェントシステム、ドキュメント生成ツール、IDE統合といった多様なAIエージェントを構築できます。

AIエージェントの活用はまだ始まったばかりです。本章がClaude Agent SDKを使ったエージェント開発の第一歩となれば幸いです。

== 参考文献

 * Building effective agents：https://www.anthropic.com/engineering/building-effective-agents
 * Agent SDK 概要（日本語）：https://platform.claude.com/docs/ja/agent-sdk/overview
 * Agent SDK 概要（英語版）：https://platform.claude.com/docs/en/agent-sdk/overview
 * Claude Code SDK Demo：https://github.com/anthropics/claude-agent-sdk-demos?tab=readme-ov-file
 * Apple's Xcode now supports the Claude Agent SDK：https://www.anthropic.com/news/apple-xcode-claude-agent-sdk
 * Claude Agent SDK [Full Workshop] — Thariq Shihipar, Anthropic：https://www.youtube.com/watch?v=TqC1qOfiVcQ&t=5s