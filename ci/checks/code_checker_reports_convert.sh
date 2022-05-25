#!/usr/bin/env sh

# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

set -e

echo "I'm alive! Run after megalinter!"

REPORT="report"
LOGS="${REPORT}/linters_logs"
CODE_CHECKER_RAW_LOGS="${LOGS}/codechecker_raw"

convert_report() {
	LINTER_TYPE=$1
	LINTER_LOG_PATH=$2
	if test -d "$LINTER_LOG_PATH" ||
		test -f "$LINTER_LOG_PATH"; then
		report-converter -t "${LINTER_TYPE}" \
			-o "${CODE_CHECKER_RAW_LOGS}" \
			"${LINTER_LOG_PATH}"
		echo "$LINTER_LOG_PATH exists."
	fi
}

convert_report_log() {
	LINTER_TYPE=$1
	LINTER_NAME=$2
	LINTER_LOG_PATH="${LOGS}/ERROR-${LINTER_NAME}.log"
	convert_report "${LINTER_TYPE}" "${LINTER_LOG_PATH}"
}

convert_report_log "cpplint" "CPP_CPPLINT"
convert_report_log "clang-tidy" "CPP_CLANG_TIDY"
convert_report "cppcheck" "${LOGS}/cppcheck_raw"

if test -d "${CODE_CHECKER_RAW_LOGS}"; then
	CodeChecker parse -e html \
		-o "${REPORT}/errors" \
		"${CODE_CHECKER_RAW_LOGS}" ||
		true # Ignore error code from checker.
	echo "$CODE_CHECKER_RAW_LOGS exists."
fi

if test -d "${CODE_CHECKER_RAW_LOGS}" &&
	test -n "${GITHUB_ACTIONS}"; then
	CodeChecker parse -o ./coverage/result.json \
		report/linters_logs/codechecker_raw/ |
		reviewdog -efm="[%m] %f:%l:%c: %m" \
			-diff="git diff HEAD" | sort | uniq |
		awk '!/'warnings-as-errors'/ {print} ' |
		reviewdog -efm="[%m] %f:%l:%c: %m" \
			-diff="git diff HEAD" \
			-reporter=github-pr-review

	TMPFILE=$(mktemp)
	git diff >"${TMPFILE}"
	reviewdog \
		-f=diff \
		-f.diff.strip=1 \
		-diff="git diff HEAD" \
		-reporter=github-pr-review <"${TMPFILE}"
fi
