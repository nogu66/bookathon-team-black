
= Claude Agent SDKで作るAIエージェント開発入門

== はじめに

こんんちは、noguです。普段は、社内システムの開発、AIエージェントの開発、AI推進に携わっております。

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

== TyoeScript SDK、Python SDK
Claude Agent SDKは、TypeScriptとPythonので提供されています。本章ではTypeScript SDKを採用しています。

== クイックスタート

=== 公式サイトの載っているチュートリアルを描く

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

 * @<href>{https://www.anthropic.com/en/gineering/building-effective-agents, Building effective agents}
 * @<href>{https://platform.claude.com/docs/ja/agent-sdk/overview, Agent SDK 概要（日本語）}
 * @<href>{https://platform.claude.com/docs/en/agent-sdk/overview, Agent SDK 概要（英語版）}
 * @<href>{https://github.com/anthropics/claude-agent-sdk-demos?tab=readme-ov-file, Claude Code SDK Demo}
 * @<href>{https://www.anthropic.com/news/apple-xcode-claude-agent-sdk, Apple's Xcode now supports the Claude Agent SDK}　
 * @<href>{https://www.youtube.com/watch?v=TqC1qOfiVcQ&t=5s, Claude Agent SDK [Full Workshop] — Thariq Shihipar, Anthropic}