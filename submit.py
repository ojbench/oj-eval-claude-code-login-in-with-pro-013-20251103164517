#!/usr/bin/env python3
import os
import sys
sys.path.append('submit_acmoj')
from acmoj_client import ACMOJClient

token = os.environ.get('ACMOJ_TOKEN')
if not token:
    print("Error: ACMOJ_TOKEN not set")
    sys.exit(1)

client = ACMOJClient(token)

# The git URL without credentials
git_url = "https://github.com/ojbench/oj-eval-claude-code-login-in-with-pro-013-20251103164517.git"

# Submit to problem 2671
print("Submitting to problem 2671...")
result = client.submit_git(2671, git_url)
print(f"Result: {result}")

if result and 'id' in result:
    print(f"Submission ID for 2671: {result['id']}")
