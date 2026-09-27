# 2026-09 예제 재정리 감사 기록

`PLAN_REORG.md`에 따른 예제 개명·통합·삭제 작업의 근거 자료이다.
작업은 2026-09-21에 완료되었고(개명 127건), 이 폴더는 **무엇이 어디로 갔는지**를 되짚기 위해 남긴다.

| 파일 | 내용 |
|---|---|
| `plan.csv` | 옛 이름 → 새 이름 매핑 전체 (`Cat, Old, New, Action, Note`) |
| `meta.csv` | 예제별 카테고리·이름·LOC·사용 레지스터·asm 여부·프로젝트 형식 |
| `pairs.csv` | 소스 유사도로 찾은 중복 후보 쌍 |
| `sample_dup.csv` | SampleCodes 이관분의 중복 판정 |
| `move_log.tsv` | 실제 이동 기록 |
| `reorg_log_step1.tsv` / `reorg_log_step2.tsv` | 단계별 실행 로그 |
| `uC_Examples.atsln.before` | Microchip Studio 솔루션 파일의 재정리 이전 상태 |

격리했던 `_to_delete/`(187개 파일)는 2026-09-27에 제거했다.
원본은 `H:\My Drive\20_Teaching\31_Microcontroller\99_Archive\2026_repo정리\_to_delete_20260927.zip`에 있다.
