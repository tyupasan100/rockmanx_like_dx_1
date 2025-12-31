# Git 操作メモ（個人制作用）

このプロジェクトで使用する Git / GitHub の基本操作まとめ。

---
# Git 操作（簡易）

## 作業開始前
git pull

## 変更確認
git status

## コミット
git add .
git commit -m "変更内容"

## GitHubへ送信
git push

## 状態確認
git log --oneline



## 作業開始時（必ずやる）

git status
git pull

他のPCでの作業内容を取得
競合防止のため必須

## 作業中

変更内容を確認：

git status
git diff

## コミットする時

git add .
git commit -m "説明を書く"

コミットメッセージ例
Add internal resolution system

Fix collision detection

Implement map integer grid

※ 1コミット = 1変更を意識する

## GitHubへ送信（push）

git push

※ 初回のみ：
git push -u origin main

## 別PC・別作業との同期
最新状態を取得
git pull

状態確認
git branch

## ブランチ（必要になったら）

git switch -c feature/xxx

戻る：
git switch main

※ 現在は main ブランチのみ運用

## よく使う確認コマンド

git log --oneline
git remote -v

## やってはいけないこと

pull せずに作業開始

動いていない状態で commit

1コミットで大量変更

## 運用方針（このプロジェクト）
個人制作

main ブランチ一本

こまめに commit

安定した状態だけ push

---